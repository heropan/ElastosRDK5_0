
#include "elastos/droid/internal/SizeAdaptiveLayout.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include "elastos/droid/R.h"
#include "elastos/droid/utility/StateSet.h"
#include "elastos/droid/widget/CView.h"
#include "elastos/droid/widget/internal/CSizeAdaptiveLayoutLayoutParams.h"
#include "elastos/droid/animation/CAnimatorSet.h"
#include "elastos/droid/animation/CObjectAnimator.h"

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Animation::IAnimatorSetBuilder;
using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::Graphics::Drawable::IColorDrawable;
using Elastos::Droid::Graphics::Drawable::IStateListDrawable;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Widget::CView;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

//===================================================================================
//              SizeAdaptiveLayout::BringToFrontOnEnd
//===================================================================================
CAR_INTERFACE_IMPL(SizeAdaptiveLayout::BringToFrontOnEnd, IAnimatorListener)

SizeAdaptiveLayout::BringToFrontOnEnd::BringToFrontOnEnd(
    /* [in] */ SizeAdaptiveLayout* host)
    : mHost(host)
{
}

ECode SizeAdaptiveLayout::BringToFrontOnEnd::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mHost->mCanceledAnimationCount == 0) {
        mHost->mLeavingView->SetVisibility(IView::GONE);
        mHost->mModestyPanel->SetVisibility(IView::GONE);
        mHost->mEnteringView->BringToFront();
        mHost->mEnteringView = NULL;
        mHost->mLeavingView = NULL;
    }
    else {
        mHost->mCanceledAnimationCount--;
    }

    return NOERROR;
}

ECode SizeAdaptiveLayout::BringToFrontOnEnd::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    mHost->mCanceledAnimationCount++;
    return NOERROR;
}

ECode SizeAdaptiveLayout::BringToFrontOnEnd::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    // if (DEBUG) Slogger::D(TAG, "fade animation repeated: should never happen.");
    assert(FALSE);
    return NOERROR;
}

ECode SizeAdaptiveLayout::BringToFrontOnEnd::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

//===================================================================================
//              SizeAdaptiveLayout
//===================================================================================
const String SizeAdaptiveLayout::TAG("SizeAdaptiveLayout");
const Boolean SizeAdaptiveLayout::SizeAdaptiveLayout::DEBUG = FALSE;
const Boolean SizeAdaptiveLayout::REPORT_BAD_BOUNDS = TRUE;
const Int64 SizeAdaptiveLayout::CROSSFADE_TIME = 250;

    // TypedArray indices
const Int32 SizeAdaptiveLayout::MIN_VALID_HEIGHT = R::styleable::SizeAdaptiveLayout_Layout_layout_minHeight;
const Int32 SizeAdaptiveLayout::MAX_VALID_HEIGHT = R::styleable::SizeAdaptiveLayout_Layout_layout_maxHeight;

SizeAdaptiveLayout::SizeAdaptiveLayout()
    : mCanceledAnimationCount(0)
    , mModestyPanelTop(0)
{
}

SizeAdaptiveLayout::SizeAdaptiveLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mCanceledAnimationCount(0)
    , mModestyPanelTop(0)
{
    Initialize();
}

SizeAdaptiveLayout::SizeAdaptiveLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
    , mCanceledAnimationCount(0)
    , mModestyPanelTop(0)
{
    Initialize();
}

SizeAdaptiveLayout::SizeAdaptiveLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mCanceledAnimationCount(0)
    , mModestyPanelTop(0)
{
    Initialize();
}

ECode SizeAdaptiveLayout::Init(
    /* [in] */ IContext* context)
{
    ViewGroup::Init(context);
    return Initialize();;
}

ECode SizeAdaptiveLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroup::Init(context, attrs);
    return Initialize();;
}

ECode SizeAdaptiveLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ViewGroup::Init(context, attrs, defStyle);
    return Initialize();;
}

ECode SizeAdaptiveLayout::Initialize()
{
    AutoPtr<IContext> context = GetContext();
    CView::New(context, (IView**)&mModestyPanel);

    // If the SizeAdaptiveLayout has a solid background, use it as a transition hint.
    AutoPtr<IDrawable> background = GetBackground();
    AutoPtr<IStateListDrawable> sld = IStateListDrawable::Probe(background.Get());
    if (sld != NULL) {
        Boolean result;
        sld->SetState(StateSet::WILD_CARD, &result);
        background = NULL;
        sld->GetCurrent((IDrawable**)&background);
    }

    AutoPtr<IColorDrawable> cdb = IColorDrawable::Probe(background.Get());
    if (cdb) {
        mModestyPanel->SetBackgroundDrawable(background);
    }
    else {
        mModestyPanel->SetBackgroundColor(IColor::BLACK);
    }

    AutoPtr<ISizeAdaptiveLayoutLayoutParams> layout;
    CSizeAdaptiveLayoutLayoutParams::New(
        IViewGroupLayoutParams::MATCH_PARENT,
        IViewGroupLayoutParams::MATCH_PARENT,
        (ISizeAdaptiveLayoutLayoutParams**)&layout);
    mModestyPanel->SetLayoutParams(layout);
    AddView(mModestyPanel);

    mAnimatorListener = new BringToFrontOnEnd(this);

    AutoPtr<ArrayOf<Float> > param = ArrayOf<Float>::Alloc(1);
    param->Set(0, 0.f);
    mFadePanel = CObjectAnimator::OfFloat(mModestyPanel, String("alpha"), param);
    mFadeView = CObjectAnimator::OfFloat(NULL, String("alpha"), param);
    assert(mFadePanel != NULL && mFadeView != NULL);
    CAnimatorSet::New((IAnimatorSet**)&mTransitionAnimation);
    AutoPtr<IAnimatorSetBuilder> build;
    mTransitionAnimation->Play(mFadeView, (IAnimatorSetBuilder**)&build);
    assert(build != NULL);
    build->With(mFadePanel);
    mTransitionAnimation->SetDuration(CROSSFADE_TIME);
    mTransitionAnimation->AddListener(mAnimatorListener);
    return NOERROR;
}

AutoPtr<IAnimator> SizeAdaptiveLayout::GetTransitionAnimation()
{
    return mTransitionAnimation;
}

AutoPtr<IView> SizeAdaptiveLayout::GetModestyPanel()
{
    return mModestyPanel;
}

ECode SizeAdaptiveLayout::OnAttachedToWindow()
{
    mLastActive = NULL;
    // make sure all views start off invisible.
    for (Int32 i = 0; i < GetChildCount(); i++) {
        AutoPtr<IView> child = GetChildAt(i);
        child->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

void SizeAdaptiveLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    // if (DEBUG) Slogger::D(TAG, this + " measure spec: " +
    //                  MeasureSpec.toString(heightMeasureSpec));
    AutoPtr<IView> model = SelectActiveChild(heightMeasureSpec);

    AutoPtr<IViewGroupLayoutParams> vglp;
    model->GetLayoutParams((IViewGroupLayoutParams**)&vglp);
    AutoPtr<ISizeAdaptiveLayoutLayoutParams> lp =
    ISizeAdaptiveLayoutLayoutParams::Probe(vglp.Get());

    if (DEBUG) {
        Int32 minHeight, maxHeight;
        lp->GetMinHeight(&minHeight);
        lp->GetMaxHeight(&maxHeight);
        Slogger::D(TAG, "active min: %d, max: %d", minHeight, maxHeight);
    }

    MeasureChild(model, widthMeasureSpec, heightMeasureSpec);
    Int32 childHeight, childWidth, state;
    model->GetMeasuredHeight(&childHeight);
    model->GetMeasuredHeight(&childWidth);
    model->GetMeasuredState(&state);

    Int32 childState = CombineMeasuredStates(0, state);

    if (DEBUG) Slogger::D(TAG, "measured child at: %d", childHeight);

    Int32 resolvedWidth = ResolveSizeAndState(childWidth, widthMeasureSpec, childState);
    Int32 resolvedHeight = ResolveSizeAndState(childHeight, heightMeasureSpec, childState);

    if (DEBUG) Slogger::D(TAG, "resolved to: %d", resolvedHeight);

    Int32 boundedHeight = ClampSizeToBounds(resolvedHeight, model);
    if (DEBUG) Slogger::D(TAG, "bounded to: %d", boundedHeight);
    SetMeasuredDimension(resolvedWidth, boundedHeight);
}

Int32 SizeAdaptiveLayout::ClampSizeToBounds(
    /* [in] */ Int32 measuredHeight,
    /* [in] */ IView* child)
{
    assert(child != NULL);

    AutoPtr<IViewGroupLayoutParams> vglp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&vglp);
    AutoPtr<ISizeAdaptiveLayoutLayoutParams> lp =
    ISizeAdaptiveLayoutLayoutParams::Probe(vglp.Get());

    Int32 minHeight, maxHeight;
    lp->GetMinHeight(&minHeight);
    lp->GetMaxHeight(&maxHeight);
    Int32 heightIn = IView::MEASURED_SIZE_MASK & measuredHeight;
    Int32 height = Elastos::Core::Math::Max(heightIn, minHeight);
    if (maxHeight != ISizeAdaptiveLayoutLayoutParams::UNBOUNDED) {
        height = Elastos::Core::Math::Min(height, maxHeight);
    }

    if (REPORT_BAD_BOUNDS && heightIn != height) {
        Slogger::D(TAG, "%p child view %p measured out of bounds at %d px clamped to %d px.",
            this, child, heightIn, height);
    }

    return height;
}

//TODO extend to width and height
AutoPtr<IView> SizeAdaptiveLayout::SelectActiveChild(
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    AutoPtr<IView> unboundedView;
    AutoPtr<IView> tallestView;
    Int32 tallestViewSize = 0;
    AutoPtr<IView> smallestView ;
    Int32 smallestViewSize = Elastos::Core::Math::INT32_MAX_VALUE;
    Int32 minHeight, maxHeight;

    for (Int32 i = 0; i < GetChildCount(); i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (child != mModestyPanel) {
            AutoPtr<IViewGroupLayoutParams> vglp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&vglp);
            AutoPtr<ISizeAdaptiveLayoutLayoutParams> lp =
            ISizeAdaptiveLayoutLayoutParams::Probe(vglp.Get());

            lp->GetMinHeight(&minHeight);
            lp->GetMaxHeight(&maxHeight);
            if (DEBUG) {
                Slogger::D(TAG, "looking at %d with min: %d, max: %d",
                    i, minHeight, maxHeight);
            }

            if (maxHeight == ISizeAdaptiveLayoutLayoutParams::UNBOUNDED
                && unboundedView == NULL) {
                unboundedView = child;
            }
            if (maxHeight > tallestViewSize) {
                tallestViewSize = maxHeight;
                tallestView = child;
            }
            if (minHeight < smallestViewSize) {
                smallestViewSize = minHeight;
                smallestView = child;
            }

            if (heightMode != MeasureSpec::UNSPECIFIED &&
                heightSize >= minHeight && heightSize <= maxHeight) {
                if (DEBUG) Slogger::D(TAG, "  found exact match, finishing early");
                return child;
            }
        }
    }

    if (unboundedView != NULL) {
        tallestView = unboundedView;
    }

    if (heightMode == MeasureSpec::UNSPECIFIED) {
        return tallestView;
    }
    if (heightSize > tallestViewSize) {
        return tallestView;
    }
    return smallestView;
}

void SizeAdaptiveLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (DEBUG) Slogger::D(TAG, "%p onlayout height: %d", this, (bottom - top));
    mLastActive = mActiveChild;
    Int32 measureSpec = View::MeasureSpec::MakeMeasureSpec(
        bottom - top,
        View::MeasureSpec::EXACTLY);
    mActiveChild = SelectActiveChild(measureSpec);
    mActiveChild->SetVisibility(IView::VISIBLE);

    if (mLastActive != mActiveChild && mLastActive != NULL) {
        if (DEBUG) Slogger::D(TAG, "%p changed children from: %p to %p",
            this, mLastActive.Get(), mActiveChild.Get());

        mEnteringView = mActiveChild;
        mLeavingView = mLastActive;

        mEnteringView->SetAlpha(1.0f);

        mModestyPanel->SetAlpha(1.0f);
        mModestyPanel->BringToFront();
        mLeavingView->GetHeight(&mModestyPanelTop);
        mModestyPanel->SetVisibility(IView::VISIBLE);
        // TODO: mModestyPanel background should be compatible with mLeavingView

        mLeavingView->BringToFront();

        if (mTransitionAnimation != NULL) {
            Boolean res;
            mTransitionAnimation->IsRunning(&res);
            if (res) {
                mTransitionAnimation->Cancel();
            }

            mFadeView->SetTarget((IInterface*)mLeavingView);
            AutoPtr<ArrayOf<Float> > fv = ArrayOf<Float>::Alloc(1);
            fv->Set(0, 0.f);
            mFadeView->SetFloatValues(fv);
            mFadePanel->SetFloatValues(fv);
            mTransitionAnimation->SetupStartValues();
            mTransitionAnimation->Start();
        }
    }

    Int32 childWidth;
    mActiveChild->GetMeasuredWidth(&childWidth);

    Int32 childHeight;
    mActiveChild->GetMeasuredHeight(&childHeight);
    // TODO investigate setting LAYER_TYPE_HARDWARE on mLastActive
    mActiveChild->Layout(0, 0, 0 + childWidth, 0 + childHeight);

    if (DEBUG) Slogger::D(TAG, "got modesty offset of %d", mModestyPanelTop);
    mModestyPanel->Layout(0, mModestyPanelTop, 0 + childWidth, mModestyPanelTop + childHeight);
}

ECode SizeAdaptiveLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    if (DEBUG) Slogger::D(TAG, "generate layout from attrs");
    AutoPtr<ISizeAdaptiveLayoutLayoutParams> lp;
    FAIL_RETURN(CSizeAdaptiveLayoutLayoutParams::New(GetContext(), attrs, (ISizeAdaptiveLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> SizeAdaptiveLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    if (DEBUG) Slogger::D(TAG, "generate default layout from viewgroup");

    AutoPtr<ISizeAdaptiveLayoutLayoutParams> lp;
    CSizeAdaptiveLayoutLayoutParams::New(p, (ISizeAdaptiveLayoutLayoutParams**)&lp);
    AutoPtr<IViewGroupLayoutParams> vglp = IViewGroupLayoutParams::Probe(lp);
    return vglp;
}

AutoPtr<IViewGroupLayoutParams> SizeAdaptiveLayout::GenerateDefaultLayoutParams()
{
    if (DEBUG) Slogger::D(TAG, "generate default layout from NULL");

    AutoPtr<IViewGroupLayoutParams> lp;
    CSizeAdaptiveLayoutLayoutParams::New((ISizeAdaptiveLayoutLayoutParams**)&lp);
    AutoPtr<IViewGroupLayoutParams> vglp = IViewGroupLayoutParams::Probe(lp);
    return vglp;
}

Boolean SizeAdaptiveLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return ISizeAdaptiveLayoutLayoutParams::Probe(p) != NULL;
}


}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

