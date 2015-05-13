
#include "CCandidatesContainer.h"
#include "CDecodingInfo.h"
#include "CPinyinEnvironmentHelper.h"
#include "view/animation/CAnimationSet.h"
#include "view/animation/CTranslateAnimation.h"
#include "view/animation/CAlphaAnimation.h"

using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::Animation::EIID_IAnimationListener;
using Elastos::Droid::View::Animation::IAnimationSet;
using Elastos::Droid::View::Animation::CAnimationSet;
using Elastos::Droid::View::Animation::CTranslateAnimation;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::ITranslateAnimation;
using Elastos::Droid::View::Animation::IAlphaAnimation;


namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

Int32 CandidatesContainer::ARROW_ALPHA_ENABLED = 0xff;
Int32 CandidatesContainer::ARROW_ALPHA_DISABLED = 0x40;
Int32 CandidatesContainer::ANIMATION_TIME = 200;

CandidatesContainer::CandidatesContainer()
    : mXOffsetForFlipper(0)
    , mCurrentPage(-1)
{
}

ECode CandidatesContainer::Initialize(
    /* [in] */ ICandidateViewListener* cvListener,
    /* [in] */ IBalloonHint* balloonHint,
    /* [in] */ IGestureDetector* gestureDetector)
{
    mCvListener = cvListener;

    mLeftArrowBtn = IImageButton::Probe(FindViewById(0x7f0a0001 /*R::id::arrow_left_btn*/));
    mRightArrowBtn = IImageButton::Probe(FindViewById(0x7f0a0002/*R::id::arrow_right_btn*/));
    mLeftArrowBtn->SetOnTouchListener((IViewOnTouchListener*)this->Probe(EIID_IViewOnTouchListener));
    mRightArrowBtn->SetOnTouchListener((IViewOnTouchListener*)this->Probe(EIID_IViewOnTouchListener));

    mFlipper = IViewFlipper::Probe(FindViewById(0x7f0a0003 /*R::id::candidate_flipper*/));
    mFlipper->SetMeasureAllChildren(TRUE);

    RelativeLayout::Invalidate();
    RequestLayout();

    Int32 count = 0;
    mFlipper->GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> view;
        mFlipper->GetChildAt(i, (IView**)&view);
        AutoPtr<ICandidateView> cv = ICandidateView::Probe(view);
        cv->Initialize(THIS_PROBE(IArrowUpdater), balloonHint, gestureDetector, mCvListener);
    }
    return NOERROR;
}

ECode CandidatesContainer::ShowCandidates(
    /* [in] */ IDecodingInfo* decInfo,
    /* [in] */ Boolean enableActiveHighlight)
{
    if (NULL == decInfo) return NOERROR;
    mDecInfo = decInfo;
    mCurrentPage = 0;

    Boolean empty = FALSE;
    if (decInfo->IsCandidatesListEmpty(&empty), empty) {
        ShowArrow(mLeftArrowBtn, FALSE);
        ShowArrow(mRightArrowBtn, FALSE);
    } else {
        ShowArrow(mLeftArrowBtn, TRUE);
        ShowArrow(mRightArrowBtn, TRUE);
    }

    Int32 count = 0;
    mFlipper->GetChildCount(&count);
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<ICandidateView> cv;
        mFlipper->GetChildAt(i, (IView**)&cv);
        cv->SetDecodingInfo(mDecInfo);
    }
    StopAnimation();

    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);
    cv->ShowPage(mCurrentPage, 0, enableActiveHighlight);

    UpdateArrowStatus();
    return RelativeLayout::Invalidate();
}

ECode CandidatesContainer::GetCurrentPage(
    /* [out] */ Int32* page)
{
    VALIDATE_NOT_NULL(page);
    *page = mCurrentPage;
    return NOERROR;
}

ECode CandidatesContainer::EnableActiveHighlight(
    /* [in] */ Boolean enableActiveHighlight)
{
    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);
    cv->EnableActiveHighlight(enableActiveHighlight);
    return RelativeLayout::Invalidate();
}

void CandidatesContainer::OnMeasure(
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
    measuredHeight += (env->GetHeightForCandidates(&value), value);
    widthMeasureSpec = MeasureSpec::MakeMeasureSpec(measuredWidth,
            MeasureSpec::EXACTLY);
    heightMeasureSpec = MeasureSpec::MakeMeasureSpec(measuredHeight,
            MeasureSpec::EXACTLY);
    RelativeLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (NULL != mLeftArrowBtn) {
        mLeftArrowBtn->GetMeasuredWidth(&mXOffsetForFlipper);
    }
}

ECode CandidatesContainer::ActiveCurseBackward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean tmp = FALSE;
    if ((mFlipper->IsFlipping(&tmp), tmp) || NULL == mDecInfo) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);
    assert(cv != NULL);

    Boolean state = FALSE;
    if (cv->ActiveCurseBackward(&state), state) {
        cv->InvalidateEx2();
        *result = TRUE;
        return NOERROR;
    }
    return PageBackward(TRUE, TRUE, result);
}

ECode CandidatesContainer::ActiveCurseForward(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean state = FALSE;
    if ((mFlipper->IsFlipping(&state), state) || NULL == mDecInfo) {
        *result = FALSE;
        return NOERROR;
    }

    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);

    if (cv->ActiveCursorForward(&state), state) {
        cv->InvalidateEx2();
        *result = TRUE;
        return NOERROR;
    }
    return PageForward(TRUE, TRUE, result);
}

ECode CandidatesContainer::PageBackward(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean enableActiveHighlight,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (NULL == mDecInfo) {
        *result = FALSE;
        return NOERROR;
    }

    Boolean state = FALSE;
    if ((mFlipper->IsFlipping(&state), state) || 0 == mCurrentPage) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 child = 0;
    mFlipper->GetDisplayedChild(&child);
    Int32 childNext = (child + 1) % 2;
    AutoPtr<ICandidateView> cv;
    mFlipper->GetChildAt(child, (IView**)&cv);
    AutoPtr<ICandidateView> cvNext;
    mFlipper->GetChildAt(childNext, (IView**)&cvNext);

    mCurrentPage--;
    Int32 activeCandInPage = 0;
    cv->GetActiveCandiatePosInPage(&activeCandInPage);
    if (animLeftRight) {
        activeCandInPage = ((CDecodingInfo*)mDecInfo.Get())->mPageStart[mCurrentPage + 1]
                - ((CDecodingInfo*)mDecInfo.Get())->mPageStart[mCurrentPage] - 1;
    }

    cvNext->ShowPage(mCurrentPage, activeCandInPage, enableActiveHighlight);
    LoadAnimation(animLeftRight, FALSE);
    StartAnimationEx();

    UpdateArrowStatus();
    *result = TRUE;
    return NOERROR;
}

ECode CandidatesContainer::PageForward(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean enableActiveHighlight,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (NULL == mDecInfo) {
        *result = FALSE;
        return NOERROR;
    }

    Boolean state = FALSE;
    if ((mFlipper->IsFlipping(&state), state) || !(mDecInfo->PreparePage(mCurrentPage + 1, &state), state)) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 child = 0;
    mFlipper->GetDisplayedChild(&child);
    Int32 childNext = (child + 1) % 2;
    AutoPtr<ICandidateView> cv;
    mFlipper->GetChildAt(child, (IView**)&cv);
    Int32 activeCandInPage = 0;
    cv->GetActiveCandiatePosInPage(&activeCandInPage);
    cv->EnableActiveHighlight(enableActiveHighlight);

    AutoPtr<ICandidateView> cvNext;
    mFlipper->GetChildAt(childNext, (IView**)&cvNext);
    mCurrentPage++;
    if (animLeftRight) activeCandInPage = 0;

    cvNext->ShowPage(mCurrentPage, activeCandInPage, enableActiveHighlight);
    LoadAnimation(animLeftRight, TRUE);
    StartAnimationEx();

    UpdateArrowStatus();
    *result = TRUE;
    return NOERROR;
}

ECode CandidatesContainer::GetActiveCandiatePos(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    if (NULL == mDecInfo) {
        *pos = -1;
        return NOERROR;
    }
    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);
    return cv->GetActiveCandiatePosGlobal(pos);
}

ECode CandidatesContainer::UpdateArrowStatus()
{
    if (mCurrentPage < 0) return NOERROR;
    Boolean forwardEnabled = FALSE;
    mDecInfo->PageForwardable(mCurrentPage, &forwardEnabled);
    Boolean backwardEnabled = FALSE;
    mDecInfo->PageBackwardable(mCurrentPage, &backwardEnabled);

    if (backwardEnabled) {
        EnableArrow(mLeftArrowBtn, TRUE);
    } else {
        EnableArrow(mLeftArrowBtn, FALSE);
    }
    if (forwardEnabled) {
        EnableArrow(mRightArrowBtn, TRUE);
    } else {
        EnableArrow(mRightArrowBtn, FALSE);
    }
    return NOERROR;
}

void CandidatesContainer::EnableArrow(
    /* [in] */ IImageButton* arrowBtn,
    /* [in] */ Boolean enabled)
{
    assert(arrowBtn != NULL);
    arrowBtn->SetEnabled(enabled);
    if (enabled)
        arrowBtn->SetAlpha(ARROW_ALPHA_ENABLED);
    else
        arrowBtn->SetAlpha(ARROW_ALPHA_DISABLED);
}

void CandidatesContainer::ShowArrow(
    /* [in] */ IImageButton* arrowBtn,
    /* [in] */ Boolean show)
{
    assert(arrowBtn != NULL);
    if (show)
        arrowBtn->SetVisibility(IView::VISIBLE);
    else
        arrowBtn->SetVisibility(IView::INVISIBLE);
}

Boolean CandidatesContainer::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    assert(event != NULL);
    event->OffsetLocation(-mXOffsetForFlipper, 0);
    AutoPtr<ICandidateView> cv;
    mFlipper->GetCurrentView((IView**)&cv);
    Boolean result = FALSE;
    cv->OnTouchEventReal(event, &result);
    return TRUE;
}

ECode CandidatesContainer::LoadAnimation(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean forward)
{
    if (animLeftRight) {
        if (forward) {
            if (NULL == mInAnimPushLeft) {
                mInAnimPushLeft = CreateAnimation(1.0f, 0, 0, 0, 0, 1.0f,
                        ANIMATION_TIME);
                mOutAnimPushLeft = CreateAnimation(0, -1.0f, 0, 0, 1.0f, 0,
                        ANIMATION_TIME);
            }
            mInAnimInUse = mInAnimPushLeft;
            mOutAnimInUse = mOutAnimPushLeft;
        } else {
            if (NULL == mInAnimPushRight) {
                mInAnimPushRight = CreateAnimation(-1.0f, 0, 0, 0, 0, 1.0f,
                        ANIMATION_TIME);
                mOutAnimPushRight = CreateAnimation(0, 1.0f, 0, 0, 1.0f, 0,
                        ANIMATION_TIME);
            }
            mInAnimInUse = mInAnimPushRight;
            mOutAnimInUse = mOutAnimPushRight;
        }
    } else {
        if (forward) {
            if (NULL == mInAnimPushUp) {
                mInAnimPushUp = CreateAnimation(0, 0, 1.0f, 0, 0, 1.0f,
                        ANIMATION_TIME);
                mOutAnimPushUp = CreateAnimation(0, 0, 0, -1.0f, 1.0f, 0,
                        ANIMATION_TIME);
            }
            mInAnimInUse = mInAnimPushUp;
            mOutAnimInUse = mOutAnimPushUp;
        } else {
            if (NULL == mInAnimPushDown) {
                mInAnimPushDown = CreateAnimation(0, 0, -1.0f, 0, 0, 1.0f,
                        ANIMATION_TIME);
                mOutAnimPushDown = CreateAnimation(0, 0, 0, 1.0f, 1.0f, 0,
                        ANIMATION_TIME);
            }
            mInAnimInUse = mInAnimPushDown;
            mOutAnimInUse = mOutAnimPushDown;
        }
    }

    mInAnimInUse->SetAnimationListener(THIS_PROBE(IAnimationListener));

    mFlipper->SetInAnimation(mInAnimInUse);
    mFlipper->SetOutAnimation(mOutAnimInUse);
    return NOERROR;
}

AutoPtr<IAnimation> CandidatesContainer::CreateAnimation(
    /* [in] */ Float xFrom,
    /* [in] */ Float xTo,
    /* [in] */ Float yFrom,
    /* [in] */ Float yTo,
    /* [in] */ Float alphaFrom,
    /* [in] */ Float alphaTo,
    /* [in] */ Int64 duration)
{
    AutoPtr<IAnimationSet> animSet;
    CAnimationSet::New(GetContext(), NULL, (IAnimationSet**)&animSet);
    AutoPtr<IAnimation> trans;
    CTranslateAnimation::New(IAnimation::RELATIVE_TO_SELF,
            xFrom, IAnimation::RELATIVE_TO_SELF, xTo,
            IAnimation::RELATIVE_TO_SELF, yFrom, IAnimation::RELATIVE_TO_SELF,
            yTo, (ITranslateAnimation**)&trans);

    AutoPtr<IAnimation> alpha;
    CAlphaAnimation::New(alphaFrom, alphaTo, (IAlphaAnimation**)&alpha);
    animSet->AddAnimation(trans);
    animSet->AddAnimation(alpha);
    animSet->SetDuration(duration);
    return animSet;
}

void CandidatesContainer::StartAnimationEx()
{
    mFlipper->ShowNext();
}

void CandidatesContainer::StopAnimation()
{
    mFlipper->StopFlipping();
}


//class CCandidatesContainer
IVIEW_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IVIEWGROUP_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IVIEWPARENT_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IVIEWMANAGER_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IDRAWABLECALLBACK_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IKEYEVENTCALLBACK_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CCandidatesContainer, CandidatesContainer)

PInterface CCandidatesContainer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CCandidatesContainer::Probe(riid);
}

ECode CCandidatesContainer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return CandidatesContainer::Init(context, attrs);
}

ECode CCandidatesContainer::UpdateArrowStatus()
{
    return CandidatesContainer::UpdateArrowStatus();
}

ECode CCandidatesContainer::OnAnimationStart(
    /* [in] */  IAnimation* animation)
{
    return NOERROR;
}

ECode CCandidatesContainer::OnAnimationEnd(
    /* [in] */  IAnimation* animation)
{
    Boolean pressed = FALSE;
    if (!(mLeftArrowBtn->IsPressed(&pressed), pressed) && !(mRightArrowBtn->IsPressed(&pressed), pressed)) {
        AutoPtr<ICandidateView> cv;
        mFlipper->GetCurrentView((IView**)&cv);
        cv->EnableActiveHighlight(TRUE);
    }
    return NOERROR;
}

ECode CCandidatesContainer::OnAnimationRepeat(
    /* [in] */  IAnimation* animation)
{
    return NOERROR;
}

ECode CCandidatesContainer::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 action = 0;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_DOWN) {
        if (v == mLeftArrowBtn) {
            mCvListener->OnToRightGesture();
        } else if (v == mRightArrowBtn) {
            mCvListener->OnToLeftGesture();
        }
    } else if (action == IMotionEvent::ACTION_UP) {
        AutoPtr<ICandidateView> cv;
        mFlipper->GetCurrentView((IView**)&cv);
        cv->EnableActiveHighlight(TRUE);
    }

    *result = FALSE;
    return NOERROR;
}

ECode CCandidatesContainer::Initialize(
    /* [in] */ ICandidateViewListener* cvListener,
    /* [in] */ IBalloonHint* balloonHint,
    /* [in] */ IGestureDetector* gestureDetector)
{
    return CandidatesContainer::Initialize(cvListener, balloonHint, gestureDetector);
}

ECode CCandidatesContainer::ShowCandidates(
    /* [in] */ IDecodingInfo* decInfo,
    /* [in] */ Boolean enableActiveHighlight)
{
    return CandidatesContainer::ShowCandidates(decInfo, enableActiveHighlight);
}

ECode CCandidatesContainer::GetCurrentPage(
    /* [out] */ Int32* page)
{
    return CandidatesContainer::GetCurrentPage(page);
}

ECode CCandidatesContainer::EnableActiveHighlight(
    /* [in] */ Boolean enableActiveHighlight)
{
    return CandidatesContainer::EnableActiveHighlight(enableActiveHighlight);
}

ECode CCandidatesContainer::ActiveCurseBackward(
    /* [out] */ Boolean* result)
{
    return CandidatesContainer::ActiveCurseBackward(result);
}

ECode CCandidatesContainer::ActiveCurseForward(
    /* [out] */ Boolean* result)
{
    return CandidatesContainer::ActiveCurseForward(result);
}

ECode CCandidatesContainer::PageBackward(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean enableActiveHighlight,
    /* [out] */ Boolean* result)
{
    return CandidatesContainer::PageBackward(animLeftRight, enableActiveHighlight, result);
}

ECode CCandidatesContainer::PageForward(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean enableActiveHighlight,
    /* [out] */ Boolean* result)
{
    return CandidatesContainer::PageForward(animLeftRight, enableActiveHighlight, result);
}

ECode CCandidatesContainer::GetActiveCandiatePos(
    /* [out] */ Int32* pos)
{
    return CandidatesContainer::GetActiveCandiatePos(pos);
}

ECode CCandidatesContainer::LoadAnimation(
    /* [in] */ Boolean animLeftRight,
    /* [in] */ Boolean forward)
{
    return CandidatesContainer::LoadAnimation(animLeftRight, forward);
}

ECode CCandidatesContainer::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return CandidatesContainer::SetIgnoreGravity(viewId);
}

ECode CCandidatesContainer::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = CandidatesContainer::GetGravity();
    return NOERROR;
}

ECode CCandidatesContainer::SetGravity(
    /* [in] */ Int32 gravity)
{
    return CandidatesContainer::SetGravity(gravity);
}

ECode CCandidatesContainer::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return CandidatesContainer::SetHorizontalGravity(horizontalGravity);
}

ECode CCandidatesContainer::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return CandidatesContainer::SetVerticalGravity(verticalGravity);
}

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos
