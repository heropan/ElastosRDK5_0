
#include "widget/internal/SlidingTab.h"
#include "view/CViewGroupLayoutParams.h"
#include "provider/Settings.h"
#include "os/UserHandle.h"
#include "graphics/CRect.h"
#include "widget/CImageView.h"
#include "widget/CTextView.h"
#include "util/CDisplayMetrics.h"
#include "view/animation/CTranslateAnimation.h"
#include "view/animation/CAlphaAnimation.h"
#include "view/animation/CLinearInterpolator.h"

using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::Animation::IAlphaAnimation;
using Elastos::Droid::View::Animation::EIID_IAnimationListener;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Provider::Settings;
//using Elastos::Droid::Widget::ImageViewScaleType;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::Animation::ILinearInterpolator;
using Elastos::Droid::View::Animation::CTranslateAnimation;
using Elastos::Droid::View::Animation::ITranslateAnimation;
using Elastos::Droid::View::Animation::CAlphaAnimation;
using Elastos::Droid::View::Animation::CLinearInterpolator;
using Elastos::Droid::View::Animation::ILinearInterpolator;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const Boolean SlidingTab::DBG;
const Int32 SlidingTab::HORIZONTAL;
const Int32 SlidingTab::VERTICAL;
const Float SlidingTab::THRESHOLD;
const Int64 SlidingTab::VIBRATE_SHORT;
const Int64 SlidingTab::VIBRATE_LONG;
const Int32 SlidingTab::TRACKING_MARGIN;
const Int32 SlidingTab::ANIM_DURATION;
const Int32 SlidingTab::ANIM_TARGET_TIME;
const String SlidingTab::TAG = String("SlidingTab");
const Int32 SlidingTab::Slider::ALIGN_LEFT;
const Int32 SlidingTab::Slider::ALIGN_RIGHT;
const Int32 SlidingTab::Slider::ALIGN_TOP;
const Int32 SlidingTab::Slider::ALIGN_BOTTOM;
const Int32 SlidingTab::Slider::STATE_NORMAL;
const Int32 SlidingTab::Slider::STATE_PRESSED;
const Int32 SlidingTab::Slider::STATE_ACTIVE;

CAR_INTERFACE_IMPL(SlidingTab::AnimationDoneListener, IAnimationListener)
CAR_INTERFACE_IMPL(SlidingTab::StartAnimationListener, IAnimationListener)

SlidingTab::AnimationDoneListener::AnimationDoneListener(
    /* [in] */ SlidingTab* host) : mHost(host)
{}

ECode SlidingTab::AnimationDoneListener::OnAnimationStart(
    /* [in] */ IAnimation* animation)
{
    return NOERROR;
}

ECode SlidingTab::AnimationDoneListener::OnAnimationEnd(
    /* [in] */ IAnimation* animation)
{
    mHost->OnAnimationDone();
    return NOERROR;
}

ECode SlidingTab::AnimationDoneListener::OnAnimationRepeat(
    /* [in] */ IAnimation* animation)
{
    return NOERROR;
}

SlidingTab::StartAnimationListener::StartAnimationListener(
    /* [in] */ SlidingTab* host,
    /* [in] */ Boolean holdAfter,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
         : mHost(host)
         , mHoldAfter(holdAfter)
         , mDx(dx)
         , mDy(dy)
{}

ECode SlidingTab::StartAnimationListener::OnAnimationStart(
    /* [in] */ IAnimation* animation)
{
    return NOERROR;
}

ECode SlidingTab::StartAnimationListener::OnAnimationEnd(
    /* [in] */ IAnimation* animation)
{
    mHost->AnimationEndInStartAnimating(mHoldAfter, mDx, mDy);
    return NOERROR;
}

ECode SlidingTab::StartAnimationListener::OnAnimationRepeat(
    /* [in] */ IAnimation* animation)
{
    return NOERROR;
}
/**
 * Constructor
 *
 * @param parent the container view of this one
 * @param tabId drawable for the tab
 * @param barId drawable for the bar
 * @param targetId drawable for the target
 */
SlidingTab::Slider::Slider(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 tabId,
    /* [in] */ Int32 barId,
    /* [in] */ Int32 targetId)
         : mCurrentState(STATE_NORMAL)
         , mAlignment(ALIGN_UNKNOWN)
         , mAlignment_value(0)
{
    // Create tab
    AutoPtr<IContext> c;
    parent->GetContext((IContext**)&c);

    CImageView::New(c, (IImageView**)&mTab);

    mTab->SetBackgroundResource(tabId);
    mTab->SetScaleType(ImageViewScaleType_CENTER);

    AutoPtr<IViewGroupLayoutParams> lp;
    CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT, (IViewGroupLayoutParams**)&lp);
    mTab->SetLayoutParams(lp);

    // Create hint TextView
    CTextView::New(c, (ITextView**)&mText);


    mText->SetLayoutParams(lp);

    mText->SetBackgroundResource(barId);
    mText->SetTextAppearance(c, R::style::TextAppearance_SlidingTabNormal);
    // hint.setSingleLine();  // Hmm.. this causes the text to disappear off-screen

    // Create target
    CImageView::New(c, (IImageView**)&mTarget);
    mTarget->SetImageResource(targetId);
    mTarget->SetScaleType(ImageViewScaleType_CENTER);
    mTarget->SetLayoutParams(lp);
    mTarget->SetVisibility(IView::INVISIBLE);

    parent->AddView(mTarget); // this needs to be first - relies on painter's algorithm
    parent->AddView(mTab);
    parent->AddView(mText);
}

void SlidingTab::Slider::SetIcon(
    /* [in] */ Int32 iconId)
{
    mTab->SetImageResource(iconId);
}

void SlidingTab::Slider::SetTabBackgroundResource(
    /* [in] */ Int32 tabId)
{
    mTab->SetBackgroundResource(tabId);
}

void SlidingTab::Slider::SetBarBackgroundResource(
    /* [in] */ Int32 barId)
{
    mText->SetBackgroundResource(barId);
}

void SlidingTab::Slider::SetHintText(
    /* [in] */ Int32 resId)
{
    mText->SetText(resId);
}

void SlidingTab::Slider::Hide()
{
    Boolean horiz = (mAlignment == ALIGN_LEFT || mAlignment == ALIGN_RIGHT);

    Int32 l, r, t, b;
    mTab->GetRight(&r);
    mTab->GetLeft(&l);
    mTab->GetTop(&t);
    mTab->GetBottom(&b);

    Int32 dx;
    dx = horiz ? (mAlignment == ALIGN_LEFT ? mAlignment_value - r
            : mAlignment_value - l) : 0;
    Int32 dy;
    dy = horiz ? 0 : (mAlignment == ALIGN_TOP ? mAlignment_value - b
            : mAlignment_value - t);

    AutoPtr<ITranslateAnimation> trans;
    CTranslateAnimation::New(0, dx, 0, dy, (ITranslateAnimation**)&trans);

    trans->SetDuration(ANIM_DURATION);
    trans->SetFillAfter(TRUE);
    mTab->StartAnimation(trans);
    mText->StartAnimation(trans);
    mTarget->SetVisibility(IView::INVISIBLE);
}

void SlidingTab::Slider::Show(
    /* [in] */ Boolean animate)
{
    mText->SetVisibility(IView::VISIBLE);
    mTab->SetVisibility(IView::VISIBLE);
    //target.setVisibility(View.INVISIBLE);
    if (animate) {

        Int32 w, h;
        mTab->GetWidth(&w);
        mTab->GetHeight(&h);

        Boolean horiz = mAlignment == ALIGN_LEFT || mAlignment == ALIGN_RIGHT;
        Int32 dx;
        dx = horiz ? (mAlignment == ALIGN_LEFT ? w : -w) : 0;
        Int32 dy;
        dy = horiz ? 0: (mAlignment == ALIGN_TOP ? h : -h);

        AutoPtr<ITranslateAnimation> trans;
        CTranslateAnimation::New(-dx, 0, -dy, 0, (ITranslateAnimation**)&trans);
        trans->SetDuration(ANIM_DURATION);
        mTab->StartAnimation(trans);
        mText->StartAnimation(trans);
    }
}

void SlidingTab::Slider::SetState(
    /* [in] */ Int32 state)
{
    mText->SetPressed(state == STATE_PRESSED);
    mTab->SetPressed(state == STATE_PRESSED);
    if (state == STATE_ACTIVE) {
        AutoPtr<ArrayOf<Int32> > activeState = ArrayOf<Int32>::Alloc(1);
        (*activeState)[0] = R::attr::state_active;
        AutoPtr<IDrawable> drawable;
        mText->GetBackground((IDrawable**)&drawable);

        Boolean res;
        drawable->IsStateful(&res);
        if (res) {
            Boolean temp;
            drawable->SetState(activeState, &temp);
        }
        drawable = NULL;
        mTab->GetBackground((IDrawable**)&drawable);

        drawable->IsStateful(&res);
        if (res) {
            Boolean temp;
            drawable->SetState(activeState, &temp);
        }

        AutoPtr<IContext> c;
        mText->GetContext((IContext**)&c);
        mText->SetTextAppearance(c, R::style::TextAppearance_SlidingTabActive);
    } else {

        AutoPtr<IContext> c;
        mText->GetContext((IContext**)&c);

        mText->SetTextAppearance(c, R::style::TextAppearance_SlidingTabNormal);
    }
    mCurrentState = state;
}

void SlidingTab::Slider::ShowTarget()
{
    AutoPtr<IAlphaAnimation> alphaAnim;
    CAlphaAnimation::New(0.0f, 1.0f, (IAlphaAnimation**)&alphaAnim);
    alphaAnim->SetDuration(ANIM_TARGET_TIME);
    mTarget->StartAnimation(alphaAnim);
    mTarget->SetVisibility(IView::VISIBLE);
}

void SlidingTab::Slider::Reset(
    /* [in] */ Boolean animate)
{
    SetState(STATE_NORMAL);
    mText->SetVisibility(IView::VISIBLE);
    AutoPtr<IContext> c;
    mText->GetContext((IContext**)&c);

    mText->SetTextAppearance(c, R::style::TextAppearance_SlidingTabNormal);

    mTab->SetVisibility(IView::VISIBLE);
    mTarget->SetVisibility(IView::INVISIBLE);
    Boolean horiz = mAlignment == ALIGN_LEFT || mAlignment == ALIGN_RIGHT;

    Int32 l, r, t, b;
    mTab->GetLeft(&l);
    mTab->GetRight(&r);
    mTab->GetTop(&t);
    mTab->GetBottom(&b);

    Int32 dx = horiz ? (mAlignment == ALIGN_LEFT ?  mAlignment_value - l
            : mAlignment_value - r) : 0;
    Int32 dy = horiz ? 0 : (mAlignment == ALIGN_TOP ? mAlignment_value - t
            : mAlignment_value - b);
    if (animate) {
        AutoPtr<ITranslateAnimation> trans;
        CTranslateAnimation::New(0, dx, 0, dy, (ITranslateAnimation**)&trans);

        trans->SetDuration(ANIM_DURATION);
        trans->SetFillAfter(FALSE);
        mText->StartAnimation(trans);
        mTab->StartAnimation(trans);
    } else {
        if (horiz) {
            mText->OffsetLeftAndRight(dx);
            mTab->OffsetLeftAndRight(dx);
        } else {
            mText->OffsetTopAndBottom(dy);
            mTab->OffsetTopAndBottom(dy);
        }
        mText->ClearAnimation();
        mTab->ClearAnimation();
        mTarget->ClearAnimation();
    }
}

void SlidingTab::Slider::SetTarget(
    /* [in] */ Int32 targetId)
{
    mTarget->SetImageResource(targetId);
}

/**
 * Layout the given widgets within the parent.
 *
 * @param l the parent's left border
 * @param t the parent's top border
 * @param r the parent's right border
 * @param b the parent's bottom border
 * @param alignment which side to align the widget to
 */
void SlidingTab::Slider::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b,
    /* [in] */ Int32 alignment)
{
    mAlignment = alignment;
    AutoPtr<IDrawable> tabBackground;
    mTab->GetBackground((IDrawable**)&tabBackground);

    Int32 handleWidth;
    tabBackground->GetIntrinsicWidth(&handleWidth);

    Int32 handleHeight;
    tabBackground->GetIntrinsicHeight(&handleHeight);

    AutoPtr<IDrawable> targetDrawable;
    mTarget->GetDrawable((IDrawable**)&targetDrawable);

    Int32 targetWidth;
    targetDrawable->GetIntrinsicWidth(&targetWidth);

    Int32 targetHeight;
    targetDrawable->GetIntrinsicHeight(&targetHeight);

    Int32 parentWidth = r - l;
    Int32 parentHeight = b - t;

    Int32 leftTarget = (Int32) (THRESHOLD * parentWidth) - targetWidth + handleWidth / 2;
    Int32 rightTarget = (Int32) ((1.0f - THRESHOLD) * parentWidth) - handleWidth / 2;
    Int32 left = (parentWidth - handleWidth) / 2;
    Int32 right = left + handleWidth;

    if (alignment == ALIGN_LEFT || alignment == ALIGN_RIGHT) {
        // horizontal
        Int32 targetTop = (parentHeight - targetHeight) / 2;
        Int32 targetBottom = targetTop + targetHeight;
        Int32 top = (parentHeight - handleHeight) / 2;
        Int32 bottom = (parentHeight + handleHeight) / 2;
        if (alignment == ALIGN_LEFT) {
            mTab->Layout(0, top, handleWidth, bottom);
            mText->Layout(0 - parentWidth, top, 0, bottom);
            mText->SetGravity(IGravity::RIGHT);
            mTarget->Layout(leftTarget, targetTop, leftTarget + targetWidth, targetBottom);
            mAlignment_value = l;
        } else {
            mTab->Layout(parentWidth - handleWidth, top, parentWidth, bottom);
            mText->Layout(parentWidth, top, parentWidth + parentWidth, bottom);
            mTarget->Layout(rightTarget, targetTop, rightTarget + targetWidth, targetBottom);
            mText->SetGravity(IGravity::TOP);
            mAlignment_value = r;
        }
    } else {
        // vertical
        Int32 targetLeft = (parentWidth - targetWidth) / 2;
        Int32 targetRight = (parentWidth + targetWidth) / 2;
        Int32 top = (Int32) (THRESHOLD * parentHeight) + handleHeight / 2 - targetHeight;
        Int32 bottom = (Int32) ((1.0f - THRESHOLD) * parentHeight) - handleHeight / 2;
        if (alignment == ALIGN_TOP) {
            mTab->Layout(left, 0, right, handleHeight);
            mText->Layout(left, 0 - parentHeight, right, 0);
            mTarget->Layout(targetLeft, top, targetRight, top + targetHeight);
            mAlignment_value = t;
        } else {
            mTab->Layout(left, parentHeight - handleHeight, right, parentHeight);
            mText->Layout(left, parentHeight, right, parentHeight + parentHeight);
            mTarget->Layout(targetLeft, bottom, targetRight, bottom + targetHeight);
            mAlignment_value = b;
        }
    }
}

void SlidingTab::Slider::UpdateDrawableStates()
{
    SetState(mCurrentState);
}

/**
 * Ensure all the dependent widgets are measured.
 */
void SlidingTab::Slider::Measure()
{
    mTab->Measure(View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED),
        View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED));
    mText->Measure(View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED),
        View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED));
}

/**
 * Get the measured tab width. Must be called after {@link Slider#measure()}.
 * @return
 */
Int32 SlidingTab::Slider::GetTabWidth()
{
    Int32 width;
    mTab->GetMeasuredWidth(&width);

    return width;
}

/**
 * Get the measured tab width. Must be called after {@link Slider#measure()}.
 * @return
 */
Int32 SlidingTab::Slider::GetTabHeight()
{
    Int32 height;
    mTab->GetMeasuredHeight(&height);
    return height;
}

/**
 * Start animating the slider. Note we need two animations since a ValueAnimator
 * keeps internal state of the invalidation region which is just the view being animated.
 *
 * @param anim1
 * @param anim2
 */
void SlidingTab::Slider::StartAnimation(
    /* [in] */ IAnimation* anim1,
    /* [in] */ IAnimation* anim2)
{
    mTab->StartAnimation(anim1);
    mText->StartAnimation(anim2);
}

void SlidingTab::Slider::HideTarget()
{
    mTarget->ClearAnimation();
    mTarget->SetVisibility(IView::INVISIBLE);
}


ECode SlidingTab::InitFromResource(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // Allocate a temporary once that can be used everywhere.
    CRect::New((IRect**)&mTmpRect);

    Int32 size = ARRAY_SIZE(R::styleable::AbsListView);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::SlidingTab, size);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, layout, (ITypedArray**)&a);

    a->GetInt32(R::styleable::SlidingTab_orientation, HORIZONTAL, &mOrientation);
    a->Recycle();

    AutoPtr<IResources> r = GetResources();
    AutoPtr<IDisplayMetrics> metrics;
    r->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    mDensity = ((CDisplayMetrics*)metrics.Get())->mDensity;
//    if (DBG) Log("- Density: " + mDensity);

    mLeftSlider = new Slider(THIS_PROBE(IViewGroup),
        R::drawable::jog_tab_left_generic,
        R::drawable::jog_tab_bar_left_generic,
        R::drawable::jog_tab_target_gray);
    mRightSlider = new Slider(THIS_PROBE(IViewGroup),
        R::drawable::jog_tab_right_generic,
        R::drawable::jog_tab_bar_right_generic,
        R::drawable::jog_tab_target_gray);

    // setBackgroundColor(0x80808080);
    return NOERROR;
}

/**
 * Constructor used when this widget is created from a layout file.
 */
SlidingTab::SlidingTab()
          : mHoldLeftOnTransition(TRUE)
          , mHoldRightOnTransition(TRUE)
          , mGrabbedState(ISlidingTabOnTriggerListener::NO_HANDLE)
          , mTriggered(FALSE)
          , mDensity(0.0f)
          , mOrientation(0)
          , mTracking(FALSE)
          , mThreshold(0.0f)
          , mAnimating(FALSE)
{}

SlidingTab::SlidingTab(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
          : ViewGroup(context, attrs)
          , mHoldLeftOnTransition(TRUE)
          , mHoldRightOnTransition(TRUE)
          , mGrabbedState(ISlidingTabOnTriggerListener::NO_HANDLE)
          , mTriggered(FALSE)
          , mDensity(0.0f)
          , mOrientation(0)
          , mTracking(FALSE)
          , mThreshold(0.0f)
          , mAnimating(FALSE)
{
    InitFromResource(context, attrs);
}

ECode SlidingTab::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroup::Init(context, attrs);
    return InitFromResource(context, attrs);
}

void SlidingTab::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthSpecMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSpecSize =  MeasureSpec::GetSize(widthMeasureSpec);

    Int32 heightSpecMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSpecSize =  MeasureSpec::GetSize(heightMeasureSpec);

    if (DBG) {
        if (widthSpecMode == MeasureSpec::UNSPECIFIED
                || heightSpecMode == MeasureSpec::UNSPECIFIED) {
//            Log.e("SlidingTab", "SlidingTab cannot have UNSPECIFIED MeasureSpec"
//                    +"(wspec=" + widthSpecMode + ", hspec=" + heightSpecMode + ")",
//                    new RuntimeException(LOG_TAG + "stack:"));
        }
    }

    mLeftSlider->Measure();
    mRightSlider->Measure();
    Int32 leftTabWidth = mLeftSlider->GetTabWidth();
    Int32 rightTabWidth = mRightSlider->GetTabWidth();
    Int32 leftTabHeight = mLeftSlider->GetTabHeight();
    Int32 rightTabHeight = mRightSlider->GetTabHeight();

    Int32 width;
    Int32 height;
    if (IsHorizontal()) {
        width = Elastos::Core::Math::Max(widthSpecSize, leftTabWidth + rightTabWidth);
        height = Elastos::Core::Math::Max(leftTabHeight, rightTabHeight);
    } else {
        width = Elastos::Core::Math::Max(leftTabWidth, rightTabHeight);
        height = Elastos::Core::Math::Max(heightSpecSize, leftTabHeight + rightTabHeight);
    }
    SetMeasuredDimension(width, height);
}

Boolean SlidingTab::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    Float x;
    event->GetX(&x);

    Float y;
    event->GetY(&y);

    if (mAnimating) {
        return FALSE;
    }

    AutoPtr<IView> leftHandle = mLeftSlider->mTab;
    leftHandle->GetHitRect(mTmpRect);
    Boolean leftHit;
    mTmpRect->Contains((Int32) x, (Int32) y, &leftHit);

    AutoPtr<IView> rightHandle = mRightSlider->mTab;
    rightHandle->GetHitRect(mTmpRect);
    Boolean rightHit;
    mTmpRect->Contains((Int32)x, (Int32) y, &rightHit);

    if (!mTracking && !(leftHit || rightHit)) {
        return FALSE;
    }

    switch (action) {
        case IMotionEvent::ACTION_DOWN: {
            mTracking = TRUE;
            mTriggered = FALSE;
            Vibrate(VIBRATE_SHORT);
            if (leftHit) {
                mCurrentSlider = mLeftSlider;
                mOtherSlider = mRightSlider;
                mThreshold = IsHorizontal() ? THRESHOLD : 1.0f - THRESHOLD;
                SetGrabbedState(ISlidingTabOnTriggerListener::LEFT_HANDLE);
            } else {
                mCurrentSlider = mRightSlider;
                mOtherSlider = mLeftSlider;
                mThreshold = IsHorizontal() ? 1.0f - THRESHOLD : THRESHOLD;
                SetGrabbedState(ISlidingTabOnTriggerListener::RIGHT_HANDLE);
            }
            mCurrentSlider->SetState(Slider::STATE_PRESSED);
            mCurrentSlider->ShowTarget();
            mOtherSlider->Hide();
            break;
        }
    }

    return TRUE;
}

/**
 * Reset the tabs to their original state and stop any existing animation.
 * Animate them back into place if animate is TRUE.
 *
 * @param animate
 */
ECode SlidingTab::Reset(
    /* [in] */ Boolean animate)
{
    mLeftSlider->Reset(animate);
    mRightSlider->Reset(animate);
    if (!animate) {
        mAnimating = FALSE;
    }
    return NOERROR;
}

ECode SlidingTab::SetVisibility(
    /* [in] */ Int32 visibility)
{
    // Clear animations so sliders don't continue to animate when we show the widget again.
    if (visibility != GetVisibility() && visibility == IView::INVISIBLE) {
       Reset(FALSE);
    }
    return ViewGroup::SetVisibility(visibility);
}

Boolean SlidingTab::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mTracking) {
        Int32 action;
        event->GetAction(&action);

        Float x;
        event->GetX(&x);

        Float y;
        event->GetY(&y);

        switch (action) {
            case IMotionEvent::ACTION_MOVE:
                if (WithinView(x, y, this) ) {
                    MoveHandle(x, y);
                    Float position = IsHorizontal() ? x : y;
                    Float target = mThreshold * (IsHorizontal() ? GetWidth() : GetHeight());
                    Boolean thresholdReached;
                    if (IsHorizontal()) {
                        thresholdReached = mCurrentSlider == mLeftSlider ?
                                position > target : position < target;
                    } else {
                        thresholdReached = mCurrentSlider == mLeftSlider ?
                                position < target : position > target;
                    }
                    if (!mTriggered && thresholdReached) {
                        mTriggered = TRUE;
                        mTracking = FALSE;
                        mCurrentSlider->SetState(Slider::STATE_ACTIVE);
                        Boolean isLeft = mCurrentSlider == mLeftSlider;
                        DispatchTriggerEvent(isLeft ?
                                ISlidingTabOnTriggerListener::LEFT_HANDLE : ISlidingTabOnTriggerListener::RIGHT_HANDLE);

                        StartAnimating(isLeft ? mHoldLeftOnTransition : mHoldRightOnTransition);
                        SetGrabbedState(ISlidingTabOnTriggerListener::NO_HANDLE);
                    }
                    break;
                }
                // Intentionally fall through - we're outside tracking rectangle

            case IMotionEvent::ACTION_UP:
            case IMotionEvent::ACTION_CANCEL:
                CancelGrab();
                break;
        }
    }

    return mTracking || ViewGroup::OnTouchEvent(event);
}

void SlidingTab::CancelGrab()
{
    mTracking = FALSE;
    mTriggered = FALSE;
    mOtherSlider->Show(TRUE);
    mCurrentSlider->Reset(FALSE);
    mCurrentSlider->HideTarget();
    mCurrentSlider = NULL;
    mOtherSlider = NULL;
    SetGrabbedState(ISlidingTabOnTriggerListener::NO_HANDLE);
}

void SlidingTab::StartAnimating(
    /* [in] */ Boolean holdAfter)
{
    mAnimating = TRUE;
    AutoPtr<IAnimation> trans1;
    AutoPtr<IAnimation> trans2;
    AutoPtr<Slider> slider = mCurrentSlider;
    AutoPtr<Slider> other = mOtherSlider;
    Int32 dx;
    Int32 dy;
    if (IsHorizontal()) {
        Int32 right;
        slider->mTab->GetRight(&right);

        Int32 width;
        slider->mTab->GetWidth(&width);

        Int32 left;
        slider->mTab->GetLeft(&left);

        Int32 viewWidth = GetWidth();
        Int32 holdOffset = holdAfter ? 0 : width; // how much of tab to show at the end of anim
        dx =  slider == mRightSlider ? - (right + viewWidth - holdOffset)
                : (viewWidth - left) + viewWidth - holdOffset;
        dy = 0;
    } else {
        Int32 top;
        slider->mTab->GetTop(&top);

        Int32 bottom;
        slider->mTab->GetBottom(&bottom);

        Int32 height;
        slider->mTab->GetHeight(&height);

        Int32 viewHeight = GetHeight();
        Int32 holdOffset = holdAfter ? 0 : height; // how much of tab to show at end of anim
        dx = 0;
        dy =  slider == mRightSlider ? (top + viewHeight - holdOffset)
                : - ((viewHeight - bottom) + viewHeight - holdOffset);
    }
    CTranslateAnimation::New(0, dx, 0, dy, (ITranslateAnimation**)&trans1);

    trans1->SetDuration(ANIM_DURATION);
    AutoPtr<ILinearInterpolator> interpolator1;
    CLinearInterpolator::New((ILinearInterpolator**)&interpolator1);
    trans1->SetInterpolator(interpolator1);
    trans1->SetFillAfter(TRUE);

    CTranslateAnimation::New(0, dx, 0, dy, (ITranslateAnimation**)&trans2);
    trans2->SetDuration(ANIM_DURATION);
    AutoPtr<ILinearInterpolator> interpolator2;
    CLinearInterpolator::New((ILinearInterpolator**)&interpolator2);
    trans2->SetInterpolator(interpolator2);
    trans2->SetFillAfter(TRUE);

    trans1->SetAnimationListener(
            new StartAnimationListener(this, holdAfter, dx, dy));

    slider->HideTarget();
    slider->StartAnimation(trans1, trans2);
}

void SlidingTab::OnAnimationDone()
{
    ResetView();
    mAnimating = FALSE;
}

Boolean SlidingTab::WithinView(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ View* view)
{
    Int32 w = view->GetHeight();
    Int32 h = view->GetWidth();

    return (IsHorizontal() && (y > - TRACKING_MARGIN) && (y < TRACKING_MARGIN + h))
        || (!IsHorizontal() && (x > -TRACKING_MARGIN) && (x < TRACKING_MARGIN + w));
}

Boolean SlidingTab::IsHorizontal()
{
    return mOrientation == HORIZONTAL;
}

void SlidingTab::ResetView()
{
    mLeftSlider->Reset(FALSE);
    mRightSlider->Reset(FALSE);
    // onLayout(TRUE, getLeft(), getTop(), getLeft() + getWidth(), getTop() + getHeight());
}

void SlidingTab::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    if (!changed) return;

    // Center the widgets in the view
    mLeftSlider->Layout(l, t, r, b, IsHorizontal() ? Slider::ALIGN_LEFT : Slider::ALIGN_BOTTOM);
    mRightSlider->Layout(l, t, r, b, IsHorizontal() ? Slider::ALIGN_RIGHT : Slider::ALIGN_TOP);
}

void SlidingTab::MoveHandle(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    AutoPtr<IView> handle = mCurrentSlider->mTab;
    AutoPtr<IView> content = mCurrentSlider->mText;
    if (IsHorizontal()) {
        Int32 l, w;
        handle->GetLeft(&l);
        handle->GetWidth(&w);

        Int32 deltaX = (Int32) x - l - (w / 2);
        handle->OffsetLeftAndRight(deltaX);
        content->OffsetLeftAndRight(deltaX);
    } else {
        Int32 t, h;
        handle->GetTop(&t);
        handle->GetHeight(&h);

        Int32 deltaY = (Int32) y - t - (h / 2);
        handle->OffsetTopAndBottom(deltaY);
        content->OffsetTopAndBottom(deltaY);
    }
    Invalidate(); // TODO: be more conservative about what we're invalidating
}

/**
 * Sets the left handle icon to a given resource.
 *
 * The resource should refer to a Drawable object, or use 0 to remove
 * the icon.
 *
 * @param iconId the resource ID of the icon drawable
 * @param targetId the resource of the target drawable
 * @param barId the resource of the bar drawable (stateful)
 * @param tabId the resource of the
 */
ECode SlidingTab::SetLeftTabResources(
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 targetId,
    /* [in] */ Int32 barId,
    /* [in] */ Int32 tabId)
{
    mLeftSlider->SetIcon(iconId);
    mLeftSlider->SetTarget(targetId);
    mLeftSlider->SetBarBackgroundResource(barId);
    mLeftSlider->SetTabBackgroundResource(tabId);
    mLeftSlider->UpdateDrawableStates();

    return NOERROR;
}

/**
 * Sets the left handle hint text to a given resource string.
 *
 * @param resId
 */
ECode SlidingTab::SetLeftHintText(
    /* [in] */ Int32 resId)
{
    if (IsHorizontal()) {
        mLeftSlider->SetHintText(resId);
    }

    return NOERROR;
}

/**
 * Sets the right handle icon to a given resource.
 *
 * The resource should refer to a Drawable object, or use 0 to remove
 * the icon.
 *
 * @param iconId the resource ID of the icon drawable
 * @param targetId the resource of the target drawable
 * @param barId the resource of the bar drawable (stateful)
 * @param tabId the resource of the
 */
ECode SlidingTab::SetRightTabResources(
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 targetId,
    /* [in] */ Int32 barId,
    /* [in] */ Int32 tabId)
{
    mRightSlider->SetIcon(iconId);
    mRightSlider->SetTarget(targetId);
    mRightSlider->SetBarBackgroundResource(barId);
    mRightSlider->SetTabBackgroundResource(tabId);
    mRightSlider->UpdateDrawableStates();

    return NOERROR;
}

/**
 * Sets the left handle hint text to a given resource string.
 *
 * @param resId
 */
ECode SlidingTab::SetRightHintText(
    /* [in] */ Int32 resId)
{
    if (IsHorizontal()) {
        mRightSlider->SetHintText(resId);
    }

    return NOERROR;
}

ECode SlidingTab::SetHoldAfterTrigger(
    /* [in] */ Boolean holdLeft,
    /* [in] */ Boolean holdRight)
{
    mHoldLeftOnTransition = holdLeft;
    mHoldRightOnTransition = holdRight;

    return NOERROR;
}

/**
 * Triggers haptic feedback.
 */
void SlidingTab::Vibrate(
    /* [in] */ Int64 duration)
{
//    Mutex::AutoLock lock(mLock);
//
//    AutoPtr<IContentResolver> cr;
//    mContext->GetContentResolver((IContentResolver**)&cr);
//    Boolean hapticEnabled = Settings::System::GetInt32ForUser(
//            cr, Settings::System::HAPTIC_FEEDBACK_ENABLED, 1,
//            UserHandle::USER_CURRENT) != 0;
//    if (hapticEnabled) {
//        if (mVibrator == NULL) {
//            mVibrator = (android.os.Vibrator) GetContext()
//                    ->GetSystemService(Context.VIBRATOR_SERVICE);
//        }
//        mVibrator->Vibrate(duration);
//    }
}

/**
 * Registers a callback to be invoked when the user triggers an event.
 *
 * @param listener the OnDialTriggerListener to attach to this view
 */
ECode SlidingTab::SetOnTriggerListener(
    /* [in] */ ISlidingTabOnTriggerListener* listener)
{
    mOnTriggerListener = listener;

    return NOERROR;
}

/**
 * Dispatches a trigger event to listener. Ignored if a listener is not set.
 * @param whichHandle the handle that triggered the event.
 */
void SlidingTab::DispatchTriggerEvent(
    /* [in] */ Int32 whichHandle)
{
//    Vibrate(VIBRATE_LONG);
    if (mOnTriggerListener != NULL) {
        mOnTriggerListener->OnTrigger(THIS_PROBE(IView), whichHandle);
    }
}

void SlidingTab::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    ViewGroup::OnVisibilityChanged(changedView, visibility);
    // When visibility changes and the user has a tab selected, unselect it and
    // make sure their callback gets called.
    if (changedView == THIS_PROBE(IView) && visibility != IView::VISIBLE
            && mGrabbedState != ISlidingTabOnTriggerListener::NO_HANDLE) {
        CancelGrab();
    }
}

/**
 * Sets the current grabbed state, and dispatches a grabbed state change
 * event to our listener.
 */
void SlidingTab::SetGrabbedState(
    /* [in] */ Int32 newState)
{
    if (newState != mGrabbedState) {
        mGrabbedState = newState;
        if (mOnTriggerListener != NULL) {
            mOnTriggerListener->OnGrabbedStateChange(THIS_PROBE(IView), mGrabbedState);
        }
    }
}

void SlidingTab::Log(
    /* [in] */ const String& msg)
{
//    Log::d(LOG_TAG, msg);
}

ECode SlidingTab::AnimationEndInStartAnimating(
    /* [in] */ Boolean holdAfter,
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    AutoPtr<IAnimation> anim;
    if(holdAfter)
    {
        CTranslateAnimation::New((Float)dx, (Float)dx, (Float)dy, (Float)dy, (ITranslateAnimation**)&anim);
        anim->SetDuration(1000);
        mAnimating = FALSE;
    } else {
        CAlphaAnimation::New(0.5f, 1.0f, (IAlphaAnimation**)&anim);
        anim->SetDuration(ANIM_DURATION);
        ResetView();
    }
    anim->SetAnimationListener(mAnimationDoneListener);
    mLeftSlider->StartAnimation(anim, anim);
    mRightSlider->StartAnimation(anim, anim);
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
