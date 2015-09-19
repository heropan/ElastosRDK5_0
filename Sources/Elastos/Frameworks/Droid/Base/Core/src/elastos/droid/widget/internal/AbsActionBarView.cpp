
#include "widget/internal/AbsActionBarView.h"
#include "animation/CObjectAnimator.h"
#include "animation/CAnimatorSet.h"
#include "view/animation/CDecelerateInterpolator.h"
#include "view/menu/CActionMenuView.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Animation::CObjectAnimator;
using Elastos::Droid::Animation::IObjectAnimator;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::IAnimatorSetBuilder;
using Elastos::Droid::View::Animation::CDecelerateInterpolator;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::View::Menu::CActionMenuView;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

AutoPtr<ITimeInterpolator> InitPolator()
{
    AutoPtr<ITimeInterpolator> value;
    CDecelerateInterpolator::New((IDecelerateInterpolator**)&value);
    return value;
}

AutoPtr<ITimeInterpolator> AbsActionBarView::sAlphaInterpolator = InitPolator();

CAR_INTERFACE_IMPL(AbsActionBarView::VisibilityAnimListener, IAnimatorListener);

AbsActionBarView::AbsActionBarView()
    : mSplitActionBar(FALSE)
    , mSplitWhenNarrow(FALSE)
    , mContentHeight(0)
{
    mVisAnimListener = new VisibilityAnimListener(this);
}

ECode AbsActionBarView::Init(
    /* [in] */ IContext* context)
{
    return ViewGroup::Init(context);
}

ECode AbsActionBarView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroup::Init(context, attrs);
}

ECode AbsActionBarView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ViewGroup::Init(context, attrs, defStyle);
}

void AbsActionBarView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    ViewGroup::OnConfigurationChanged(newConfig);

    // Action bar can change size on configuration changes.
    // Reread the desired height from the theme-specified style.
    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > values = ArrayOf<Int32>::Alloc(
        const_cast<Int32*>(R::styleable::ActionBar),
        ARRAY_SIZE(R::styleable::ActionBar));

    GetContext()->ObtainStyledAttributes(NULL, values,
        R::attr::actionBarStyle, 0, (ITypedArray**)&a);

    Int32 h = 0;
    a->GetLayoutDimension(R::styleable::ActionBar_height, 0, &h);
    SetContentHeight(h);

    a->Recycle();
    if (mSplitWhenNarrow) {

        AutoPtr<IResources> re;
        GetContext()->GetResources((IResources**)&re);

        Boolean narrow = FALSE;
        re->GetBoolean(R::bool_::split_action_bar_is_narrow, &narrow);
        SetSplitActionBar(narrow);
    }
    if (mActionMenuPresenter != NULL) {
        mActionMenuPresenter->OnConfigurationChanged(newConfig);
    }
}

ECode AbsActionBarView::SetSplitActionBar(
    /* [in] */ Boolean split)
{
    mSplitActionBar = split;

    return NOERROR;
}

ECode AbsActionBarView::SetSplitWhenNarrow(
    /* [in] */ Boolean splitWhenNarrow)
{
    mSplitWhenNarrow = splitWhenNarrow;
    return NOERROR;
}

ECode AbsActionBarView::SetContentHeight(
    /* [in] */ Int32 height)
{
    mContentHeight = height;
    if (mMenuView != NULL) {
        mMenuView->SetMaxItemHeight(mContentHeight);
    }

    RequestLayout();

    return NOERROR;
}

Int32 AbsActionBarView::GetContentHeight()
{
    return mContentHeight;
}

ECode AbsActionBarView::SetSplitView(
    /* [in] */ IActionBarContainer* splitView)
{
    mSplitView = splitView;

    return NOERROR;
}

Int32 AbsActionBarView::GetAnimatedVisibility()
{
    if (mVisibilityAnim != NULL) {
        return mVisAnimListener->mFinalVisibility;
    }
    return GetVisibility();
}

ECode AbsActionBarView::AnimateToVisibility(
    /* [in] */ Int32 visibility)
{
     if (mVisibilityAnim != NULL) {
         mVisibilityAnim->Cancel();
     }

     if (visibility == IView::VISIBLE) {
         if (GetVisibility() != IView::VISIBLE) {
             SetAlpha(0);
             if (mSplitView != NULL && mMenuView != NULL) {
                 mMenuView->SetAlpha(0);
             }
         }

         AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
         (*values)[0] = 1;
         AutoPtr<IObjectAnimator> anim = CObjectAnimator::OfFloat((IInterface*)this->Probe(EIID_IInterface), String("alpha"), values);
         anim->SetDuration(FADE_DURATION);
         anim->SetInterpolator(sAlphaInterpolator);
         if (mSplitView != NULL && mMenuView != NULL) {
             AutoPtr<IAnimatorSet> set;
             CAnimatorSet::New((IAnimatorSet**)&set);
             AutoPtr<IObjectAnimator> splitAnim = CObjectAnimator::OfFloat(mMenuView, String("alpha"), values);
             splitAnim->SetDuration(FADE_DURATION);
             set->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
             AutoPtr<IAnimatorSetBuilder> ab;
             set->Play(anim, (IAnimatorSetBuilder**)&ab);
             ab->With(splitAnim);
             set->Start();
         } else {
             anim->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
             anim->Start();
         }
     } else {
        AutoPtr<ArrayOf<Float> > values = ArrayOf<Float>::Alloc(1);
         (*values)[0] = 0;
         AutoPtr<IObjectAnimator> anim = CObjectAnimator::OfFloat((IInterface*)this->Probe(EIID_IInterface), String("alpha"), values);
         anim->SetDuration(FADE_DURATION);
         anim->SetInterpolator(sAlphaInterpolator);
         if (mSplitView != NULL && mMenuView != NULL) {
             AutoPtr<IAnimatorSet> set;
             CAnimatorSet::New((IAnimatorSet**)&set);
             AutoPtr<IObjectAnimator> splitAnim = CObjectAnimator::OfFloat(mMenuView, String("alpha"), values);
             splitAnim->SetDuration(FADE_DURATION);
             set->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
             AutoPtr<IAnimatorSetBuilder> ab;
             set->Play(anim, (IAnimatorSetBuilder**)&ab);
             ab->With(splitAnim);
             set->Start();
         } else {
             anim->AddListener(mVisAnimListener->WithFinalVisibility(visibility));
             anim->Start();
         }
     }

    return NOERROR;
}


ECode AbsActionBarView::SetVisibility(
    /* [in] */ Int32 visibility)
{
    if (visibility != GetVisibility()) {
        if (mVisibilityAnim != NULL) {
            mVisibilityAnim->End();
        }
        ViewGroup::SetVisibility(visibility);
    }

    return NOERROR;
}

Boolean AbsActionBarView::ShowOverflowMenu()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->ShowOverflowMenu(&res);
    }
    return FALSE;
}

ECode AbsActionBarView::PostShowOverflowMenu()
{
    AutoPtr<ActionBarRunnable> r = new ActionBarRunnable(this);
    Post(r);
    return NOERROR;
}

Boolean AbsActionBarView::HideOverflowMenu()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->HideOverflowMenu(&res);
    }
    return FALSE;
}

Boolean AbsActionBarView::IsOverflowMenuShowing()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        return mActionMenuPresenter->IsOverflowMenuShowing(&res);
    }
    return FALSE;
}

Boolean AbsActionBarView::IsOverflowReserved()
{
    Boolean res = FALSE;
    return mActionMenuPresenter != NULL && (mActionMenuPresenter->IsOverflowReserved(&res), res);
}

ECode AbsActionBarView::DismissPopupMenus()
{
    if (mActionMenuPresenter != NULL) {
        Boolean res = FALSE;
        mActionMenuPresenter->DismissPopupMenus(&res);
    }

    return NOERROR;
}

Int32 AbsActionBarView::MeasureChildView(
    /* [in] */ IView* child,
    /* [in] */ Int32 availableWidth,
    /* [in] */ Int32 childSpecHeight,
    /* [in] */ Int32 spacing)
{
    child->Measure(MeasureSpec::MakeMeasureSpec(availableWidth, MeasureSpec::AT_MOST),
            childSpecHeight);

    Int32 w = 0;
    child->GetMeasuredWidth(&w);
    availableWidth -= w;
    availableWidth -= spacing;

    return Elastos::Core::Math::Max(0, availableWidth);
}

Int32 AbsActionBarView::Next(
    /* [in] */ Int32 x,
    /* [in] */ Int32 val,
    /* [in] */ Boolean isRtl)
{
    return isRtl ? x - val : x + val;
}

Int32 AbsActionBarView::PositionChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 contentHeight,
    /* [in] */ Boolean reverse)
{
    Int32 childWidth = 0;
    child->GetMeasuredWidth(&childWidth);

    Int32 childHeight = 0;
    child->GetMeasuredHeight(&childHeight);

    Int32 childTop = y + (contentHeight - childHeight) / 2;

    if (reverse) {
        child->Layout(x - childWidth, childTop, x, childTop + childHeight);
    } else {
        child->Layout(x, childTop, x + childWidth, childTop + childHeight);
    }

    return  (reverse ? -childWidth : childWidth);
}

/////////////////////////////////////////////////////////////////
//          AbsActionBarView::VisibilityAnimListener
////////////////////////////////////////////////////////////////
AbsActionBarView::VisibilityAnimListener::VisibilityAnimListener(
    /* [in] */ AbsActionBarView* host)
    : mHost(host)
    , mCanceled(FALSE)
    , mFinalVisibility(0)
{
}

AutoPtr<AbsActionBarView::VisibilityAnimListener> AbsActionBarView::VisibilityAnimListener::WithFinalVisibility(
    /* [in] */ Int32 visibility)
{
    mFinalVisibility = visibility;
    return this;
}

ECode AbsActionBarView::VisibilityAnimListener::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    mHost->SetVisibility(IView::VISIBLE);
    mHost->mVisibilityAnim = animation;
    mCanceled = FALSE;

    return NOERROR;
}

ECode AbsActionBarView::VisibilityAnimListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mCanceled) return NOERROR;

    mHost->mVisibilityAnim = NULL;
    mHost->SetVisibility(mFinalVisibility);
    if (mHost->mSplitView != NULL && mHost->mMenuView != NULL) {
        mHost->mMenuView->SetVisibility(mFinalVisibility);
    }

    return NOERROR;
}

ECode AbsActionBarView::VisibilityAnimListener::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    mCanceled = TRUE;

    return NOERROR;
}

ECode AbsActionBarView::VisibilityAnimListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return NOERROR;
}

/////////////////////////////////////////////////////////////////
//          AbsActionBarView::ActionBarRunnable
////////////////////////////////////////////////////////////////
AbsActionBarView::ActionBarRunnable::ActionBarRunnable(
    /* [in] */ AbsActionBarView* host)
    : mHost(host)
{}

ECode AbsActionBarView::ActionBarRunnable::Run()
{
    mHost->ShowOverflowMenu();
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
