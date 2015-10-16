
#include "elastos/droid/internal/ActionBarOverlayLayout.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

AutoPtr<ArrayOf<Int32> > InitArray()
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(1);
    (*array)[0] = R::attr::actionBarSize;
    return array;
}

const AutoPtr<ArrayOf<Int32> > ActionBarOverlayLayout::mActionBarSizeAttr = InitArray();

ActionBarOverlayLayout::ActionBarOverlayLayout()
{
    mActionBarHeight = 0;
    mLastSystemUiVisibility = 0;
    mWindowVisibility = IView::VISIBLE;
    CRect::New(0, 0, 0, 0, (IRect**)&mZeroRect);
}

ActionBarOverlayLayout::ActionBarOverlayLayout(
    /* [in] */ IContext* context)
    : FrameLayout(context)
    , mWindowVisibility(IView::VISIBLE)
{
    mActionBarHeight = 0;
    mLastSystemUiVisibility = 0;
    mWindowVisibility = IView::VISIBLE;
    CRect::New(0, 0, 0, 0, (IRect**)&mZeroRect);
    InitInternal(context);
}

ActionBarOverlayLayout::ActionBarOverlayLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayout(context, attrs)
    , mWindowVisibility(IView::VISIBLE)
{
    mActionBarHeight = 0;
    mLastSystemUiVisibility = 0;
    mWindowVisibility = IView::VISIBLE;
    CRect::New(0, 0, 0, 0, (IRect**)&mZeroRect);
    InitInternal(context);
}

ECode ActionBarOverlayLayout::Init(
    /* [in] */ IContext* context)
{
    FrameLayout::Init(context);
    InitInternal(context);
    return NOERROR;
}

ECode ActionBarOverlayLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::Init(context, attrs);
    InitInternal(context);
    return NOERROR;
}

void ActionBarOverlayLayout::InitInternal(
    /* [in] */ IContext* context)
{
    AutoPtr<IResourcesTheme> theme;
    GetContext()->GetTheme((IResourcesTheme**)&theme);
    AutoPtr<ITypedArray> ta;
    theme->ObtainStyledAttributes(mActionBarSizeAttr, (ITypedArray**)&ta);

    ta->GetDimensionPixelSize(0, 0, &mActionBarHeight);
    ta->Recycle();
}

ECode ActionBarOverlayLayout::SetActionBar(
    /* [in] */ IActionBarImpl* impl)
{
    mActionBar = impl;
    if (GetWindowToken() != NULL) {
        // This is being initialized after being added to a window;
        // make sure to update all state now.
        mActionBar->SetWindowVisibility(mWindowVisibility);
        if (mLastSystemUiVisibility != 0) {
            Int32 newVis = mLastSystemUiVisibility;
            OnWindowSystemUiVisibilityChanged(newVis);
            RequestFitSystemWindows();
        }
    }

    return NOERROR;
}

ECode ActionBarOverlayLayout::SetShowingForActionMode(
    /* [in] */ Boolean showing)
{
    if (showing) {
        // Here's a fun hack: if the status bar is currently being hidden,
        // and the application has asked for stable content insets, then
        // we will end up with the action mode action bar being shown
        // without the status bar, but moved below where the status bar
        // would be.  Not nice.  Trying to have this be positioned
        // correctly is not easy (basically we need yet *another* content
        // inset from the window manager to know where to put it), so
        // instead we will just temporarily force the status bar to be shown.
        if ((GetWindowSystemUiVisibility() & (IView::SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | IView::SYSTEM_UI_FLAG_LAYOUT_STABLE))
                == (IView::SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN | IView::SYSTEM_UI_FLAG_LAYOUT_STABLE)) {
            SetDisabledSystemUiVisibility(IView::SYSTEM_UI_FLAG_FULLSCREEN);
        }
    } else {
        SetDisabledSystemUiVisibility(0);
    }

    return NOERROR;
}

ECode ActionBarOverlayLayout::OnWindowSystemUiVisibilityChanged(
    /* [in] */ Int32 visible)
{
    FrameLayout::OnWindowSystemUiVisibilityChanged(visible);
    PullChildren();
    Int32 diff = mLastSystemUiVisibility ^ visible;
    mLastSystemUiVisibility = visible;
    Boolean barVisible = (visible & IView::SYSTEM_UI_FLAG_FULLSCREEN) == 0;

    Boolean res = FALSE;
    Boolean wasVisible = mActionBar != NULL ? (mActionBar->IsSystemShowing(&res), res) : TRUE;
    if (mActionBar != NULL) {
        if (barVisible) mActionBar->ShowForSystem();
        else mActionBar->HideForSystem();
    }
    if ((diff & IView::SYSTEM_UI_FLAG_LAYOUT_STABLE) != 0) {
        if (mActionBar != NULL) {
            RequestFitSystemWindows();
        }
    }

    return NOERROR;
}

void ActionBarOverlayLayout::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    FrameLayout::OnWindowVisibilityChanged(visibility);
    mWindowVisibility = visibility;
    if (mActionBar != NULL) {
        mActionBar->SetWindowVisibility(visibility);
    }
}

Boolean ActionBarOverlayLayout::ApplyInsets(
    /* [in] */ IView* view,
    /* [in] */ IRect* insets,
    /* [in] */ Boolean left,
    /* [in] */ Boolean top,
    /* [in] */ Boolean bottom,
    /* [in] */ Boolean right)
{
    Boolean changed = FALSE;
    AutoPtr<IFrameLayoutLayoutParams> lp;
    view->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    Int32 lpValue, rValue;

    lp->GetLeftMargin(&lpValue);
    insets->GetLeft(&rValue);
    if (left && lpValue != rValue) {
        changed = TRUE;
        lp->SetLeftMargin(rValue);
    }

    lp->GetTopMargin(&lpValue);
    insets->GetTop(&rValue);
    if (top && lpValue != rValue) {
        changed = TRUE;
        lp->SetTopMargin(rValue);
    }

    lp->GetRightMargin(&lpValue);
    insets->GetRight(&rValue);
    if (right && lpValue != rValue) {
        changed = TRUE;
        lp->SetRightMargin(rValue);
    }

    lp->GetBottomMargin(&lpValue);
    insets->GetBottom(&rValue);
    if (bottom && lpValue != rValue) {
        changed = TRUE;
        lp->SetBottomMargin(rValue);
    }
    return changed;
}

Boolean ActionBarOverlayLayout::FitSystemWindows(
    /* [in] */ IRect* insets)
{
    PullChildren();

    Int32 vis = GetWindowSystemUiVisibility();
    Boolean stable = (vis & IView::SYSTEM_UI_FLAG_LAYOUT_STABLE) != 0;

    // The top and bottom action bars are always within the content area.
    Boolean changed = ApplyInsets(mActionBarTop, insets, TRUE, TRUE, FALSE, TRUE);
    if (mActionBarBottom != NULL) {
        changed |= ApplyInsets(mActionBarBottom, insets, TRUE, FALSE, TRUE, TRUE);
    }

    // If the window has not requested system UI layout flags, we need to
    // make sure its content is not being covered by system UI...  though it
    // will still be covered by the action bar since they have requested it to
    // overlay.
    if ((vis & IView::SYSTEM_UI_LAYOUT_FLAGS) == 0) {
        changed |= ApplyInsets(mContent, insets, TRUE, TRUE, TRUE, TRUE);
        // The insets are now consumed.
        insets->Set(0, 0, 0, 0);
    } else {
        changed |= ApplyInsets(mContent, mZeroRect, TRUE, TRUE, TRUE, TRUE);
    }


    Int32 visible = 0;
    mActionBarTop->GetVisibility(&visible);
    if (stable || visible == IView::VISIBLE) {
        Int32 oldValue;
        insets->GetTop(&oldValue);
        insets->SetTop(oldValue + mActionBarHeight);
        // The action bar creates additional insets for its content to use.
    }

    Boolean tab = FALSE;

    if (mActionBar != NULL && (mActionBar->HasNonEmbeddedTabs(&tab), tab)) {
        AutoPtr<IView> tabs;
        mContainerView->GetTabContainer((IView**)&tab);

        if (stable || (tabs != NULL && (tabs->GetVisibility(&visible), visible) == IView::VISIBLE)) {
            // If tabs are not embedded, adjust insets to account for them.
            Int32 oldTop;
            insets->GetTop(&oldTop);
            insets->SetTop(oldTop + mActionBarHeight);
        }
    }

    Boolean split = FALSE;
    mActionView->IsSplitActionBar(&split);
    if (split) {
        if (stable || (mActionBarBottom != NULL
                && (mActionBarBottom->GetVisibility(&visible), visible) == IView::VISIBLE)) {
            // If action bar is split, adjust buttom insets for it.
            Int32 oldBottom;
            insets->GetBottom(&oldBottom);
            insets->SetBottom(oldBottom + mActionBarHeight);
        }
    }

    if (changed) {
        RequestLayout();
    }

    return FrameLayout::FitSystemWindows(insets);
}

void ActionBarOverlayLayout::PullChildren()
{
    if (mContent == NULL) {
        mContent = FindViewById(R::id::content);
        mActionBarTop = FindViewById(R::id::top_action_bar);
        mContainerView = IActionBarContainer::Probe(FindViewById(R::id::action_bar_container));
        mActionView = IActionBarView::Probe(FindViewById(R::id::action_bar));
        mActionBarBottom = FindViewById(R::id::split_action_bar);
    }
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
