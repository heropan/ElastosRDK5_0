
#include "internal/ActionBarContainer.h"
#include <R.h>
#include "widget/CFrameLayoutLayoutParams.h"
#include <elastos/core/Math.h>

using Elastos::Core::Math;
using Elastos::Droid::App::IActionBar;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

ActionBarContainer::ActionBarContainer()
{}

ActionBarContainer::ActionBarContainer(
    /* [in] */ IContext* context)
{
    Init(context, NULL);
}

ActionBarContainer::ActionBarContainer(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ECode ActionBarContainer::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::Init(context, attrs);

    SetBackgroundDrawable(NULL);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ActionBar),
            ARRAY_SIZE(R::styleable::ActionBar));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    a->GetDrawable(R::styleable::ActionBar_background, (IDrawable**)&mBackground);

    a->GetDrawable(R::styleable::ActionBar_backgroundStacked, (IDrawable**)&mStackedBackground);

    if (GetId() == R::id::split_action_bar) {
        mIsSplit = TRUE;
        a->GetDrawable(R::styleable::ActionBar_backgroundSplit, (IDrawable**)&mSplitBackground);
    }
    a->Recycle();

    SetWillNotDraw(mIsSplit ? mSplitBackground == NULL :
            mBackground == NULL && mStackedBackground == NULL);

    return NOERROR;
}

ECode ActionBarContainer::OnFinishInflate()
{
    FrameLayout::OnFinishInflate();
    mActionBarView = IActionBarView::Probe(FindViewById(R::id::action_bar));

    return NOERROR;
}

ECode ActionBarContainer::SetPrimaryBackground(
    /* [in] */ IDrawable* bg)
{
    if (mBackground != NULL) {
        mBackground->SetCallback(NULL);
        UnscheduleDrawable(mBackground);
    }
    mBackground = bg;
    if (bg != NULL) {
        bg->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    SetWillNotDraw(mIsSplit ? mSplitBackground == NULL :
            mBackground == NULL && mStackedBackground == NULL);
    Invalidate();
    return NOERROR;
}

ECode ActionBarContainer::SetStackedBackground(
    /* [in] */ IDrawable* bg)
{
    if (mStackedBackground != NULL) {
        mStackedBackground->SetCallback(NULL);
        UnscheduleDrawable(mStackedBackground);
    }
    mStackedBackground = bg;
    if (bg != NULL) {
        bg->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    SetWillNotDraw(mIsSplit ? mSplitBackground == NULL :
            mBackground == NULL && mStackedBackground == NULL);
    Invalidate();
    return NOERROR;
}

ECode ActionBarContainer::SetSplitBackground(
    /* [in] */ IDrawable* bg)
{
    if (mSplitBackground != NULL) {
        mSplitBackground->SetCallback(NULL);
        UnscheduleDrawable(mSplitBackground);
    }
    mSplitBackground = bg;
    if (bg != NULL) {
        bg->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
    }
    SetWillNotDraw(mIsSplit ? mSplitBackground == NULL :
            mBackground == NULL && mStackedBackground == NULL);
    Invalidate();

    return NOERROR;
}

ECode ActionBarContainer::SetVisibility(
    /* [in] */ Int32 visibility)
{
    FrameLayout::SetVisibility(visibility);
    Boolean isVisible = visibility == IView::VISIBLE;
    Boolean res = FALSE;
    if (mBackground != NULL) mBackground->SetVisible(isVisible, FALSE, &res);
    if (mStackedBackground != NULL) mStackedBackground->SetVisible(isVisible, FALSE, &res);
    if (mSplitBackground != NULL) mSplitBackground->SetVisible(isVisible, FALSE, &res);

    return NOERROR;
}

Boolean ActionBarContainer::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return (who == mBackground && !mIsSplit) || (who == mStackedBackground && mIsStacked) ||
            (who == mSplitBackground && mIsSplit) || FrameLayout::VerifyDrawable(who);
}

ECode ActionBarContainer::DrawableStateChanged()
{
    FrameLayout::DrawableStateChanged();

    Boolean res = FALSE;
    AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
    Boolean b = FALSE;
    if (mBackground != NULL && (mBackground->IsStateful(&res), res)) {
        mBackground->SetState(drawableState, &b);
    }
    if (mStackedBackground != NULL && (mStackedBackground->IsStateful(&res), res)) {
        mStackedBackground->SetState(drawableState, &b);
    }
    if (mSplitBackground != NULL && (mSplitBackground->IsStateful(&res), res)) {
        mSplitBackground->SetState(drawableState, &b);
    }
    return NOERROR;
}

ECode ActionBarContainer::JumpDrawablesToCurrentState()
{
    FrameLayout::JumpDrawablesToCurrentState();
    if (mBackground != NULL) {
        mBackground->JumpToCurrentState();
    }
    if (mStackedBackground != NULL) {
        mStackedBackground->JumpToCurrentState();
    }
    if (mSplitBackground != NULL) {
        mSplitBackground->JumpToCurrentState();
    }

    return NOERROR;
}

ECode ActionBarContainer::OnResolveDrawables(
    /* [in] */ Int32 layoutDirection)
{
    FrameLayout::OnResolveDrawables(layoutDirection);
    if (mBackground != NULL) {
        mBackground->SetLayoutDirection(layoutDirection);
    }
    if (mStackedBackground != NULL) {
        mStackedBackground->SetLayoutDirection(layoutDirection);
    }
    if (mSplitBackground != NULL) {
        mSplitBackground->SetLayoutDirection(layoutDirection);
    }

    return NOERROR;
}

ECode ActionBarContainer::SetTransitioning(
    /* [in] */ Boolean isTransitioning)
{
    mIsTransitioning = isTransitioning;
    SetDescendantFocusability(isTransitioning ? FOCUS_BLOCK_DESCENDANTS
            : FOCUS_AFTER_DESCENDANTS);

    return NOERROR;
}

Boolean ActionBarContainer::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    return mIsTransitioning || FrameLayout::OnInterceptTouchEvent(ev);
}

Boolean ActionBarContainer::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    FrameLayout::OnTouchEvent(ev);

    return TRUE;
}

Boolean ActionBarContainer::OnHoverEvent(
    /* [in] */ IMotionEvent* ev)
{
    FrameLayout::OnHoverEvent(ev);

    return TRUE;
}

ECode ActionBarContainer::SetTabContainer(
    /* [in] */ IScrollingTabContainerView* tabView)
{
    if (mTabContainer != NULL) {
        RemoveView(mTabContainer);
    }
    mTabContainer = tabView;
    if (tabView != NULL) {
        AddView(tabView);
        AutoPtr<IViewGroupLayoutParams> lp;
        tabView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        lp->SetWidth(IViewGroupLayoutParams::MATCH_PARENT);
        lp->SetHeight(IViewGroupLayoutParams::WRAP_CONTENT);
        tabView->SetAllowCollapse(FALSE);
    }
    return NOERROR;
}

AutoPtr<IView> ActionBarContainer::GetTabContainer()
{
    return mTabContainer;
}

void ActionBarContainer::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if (GetWidth() == 0 || GetHeight() == 0) {
        return;
    }

    if (mIsSplit) {
        if (mSplitBackground != NULL) mSplitBackground->Draw(canvas);
    } else {
        if (mBackground != NULL) {
            mBackground->Draw(canvas);
        }
        if (mStackedBackground != NULL && mIsStacked) {
            mStackedBackground->Draw(canvas);
        }
    }
}

AutoPtr<IActionMode> ActionBarContainer::StartActionModeForChild(
    /* [in] */ IView* child,
    /* [in] */ IActionModeCallback* callback)
{
    return NULL;
}

void ActionBarContainer::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FrameLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (mActionBarView == NULL) return;

    AutoPtr<IViewGroupLayoutParams> lp;
    mActionBarView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    AutoPtr<CFrameLayoutLayoutParams> cp = (CFrameLayoutLayoutParams*)lp.Get();

    Int32 h = 0;
    mActionBarView->GetMeasuredHeight(&h);
    Boolean res = FALSE;
    Int32 actionBarViewHeight = (mActionBarView->IsCollapsed(&res), res) ? 0 : h + cp->mTopMargin + cp->mBottomMargin;

    Int32 visible = 0;
    if (mTabContainer != NULL && (mTabContainer->GetVisibility(&visible), visible) != IView::GONE) {
        Int32 mode = MeasureSpec::GetMode(heightMeasureSpec);
        if (mode == MeasureSpec::AT_MOST) {
            Int32 maxHeight = MeasureSpec::GetSize(heightMeasureSpec);
            SetMeasuredDimension(GetMeasuredWidth(),
                Elastos::Core::Math::Min(actionBarViewHeight + (mTabContainer->GetMeasuredHeight(&h), h),
                            maxHeight));
        }
    }
}

void ActionBarContainer::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    FrameLayout::OnLayout(changed, l, t, r, b);

    Int32 visible;

    Boolean hasTabs = mTabContainer != NULL && (mTabContainer->GetVisibility(&visible), visible) != IView::GONE;

    if (hasTabs) {
        Int32 containerHeight = GetMeasuredHeight();
        Int32 tabHeight;
        mTabContainer->GetMeasuredHeight(&tabHeight);

        Int32 options;
        mActionBarView->GetDisplayOptions(&options);

        if ((options & IActionBar::DISPLAY_SHOW_HOME) == 0) {
            // Not showing home, put tabs on top.
            Int32 count = GetChildCount();
            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IView> child = GetChildAt(i);

                if (child == mTabContainer) continue;

                Boolean collapsed;
                mActionBarView->IsCollapsed(&collapsed);
                if (!collapsed) {
                    child->OffsetTopAndBottom(tabHeight);
                }
            }
            mTabContainer->Layout(l, 0, r, tabHeight);
        } else {
            mTabContainer->Layout(l, containerHeight - tabHeight, r, containerHeight);
        }
    }

    Boolean needsInvalidate = FALSE;
    if (mIsSplit) {
        if (mSplitBackground != NULL) {
            mSplitBackground->SetBounds(0, 0, GetMeasuredWidth(), GetMeasuredHeight());
            needsInvalidate = TRUE;
        }
    } else {
        if (mBackground != NULL) {
            Int32 l, t, r, b;
            mActionBarView->GetLeft(&l);
            mActionBarView->GetTop(&t);
            mActionBarView->GetRight(&r);
            mActionBarView->GetBottom(&b);

            mBackground->SetBounds(l, t, r, b);
            needsInvalidate = TRUE;
        }
        if ((mIsStacked = hasTabs && mStackedBackground != NULL)) {

            Int32 l, t, r, b;
            mTabContainer->GetLeft(&l);
            mTabContainer->GetTop(&t);
            mTabContainer->GetRight(&r);
            mTabContainer->GetBottom(&b);

            mStackedBackground->SetBounds(l, t, r, b);
            needsInvalidate = TRUE;
        }
    }

    if (needsInvalidate) {
        Invalidate();
    }
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

