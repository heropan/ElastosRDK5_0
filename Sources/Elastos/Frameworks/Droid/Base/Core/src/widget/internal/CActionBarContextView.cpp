
#include "widget/internal/CActionBarContextView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IVIEWGROUP_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IVIEWPARENT_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IVIEWMANAGER_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IDRAWABLECALLBACK_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IKEYEVENTCALLBACK_METHODS_IMPL(CActionBarContextView, ActionBarContextView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CActionBarContextView, ActionBarContextView)

ECode CActionBarContextView::constructor(
    /* [in] */ IContext* ctx)
{
    return ActionBarContextView::Init(ctx);
}

ECode CActionBarContextView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return ActionBarContextView::Init(ctx, attrs);
}

ECode CActionBarContextView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ActionBarContextView::Init(ctx, attrs, defStyle);
}

PInterface CActionBarContextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CActionBarContextView::Probe(riid);
}

ECode CActionBarContextView::SetCustomView(
    /* [in] */ IView* view)
{
    return ActionBarContextView::SetCustomView(view);
}

ECode CActionBarContextView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return ActionBarContextView::SetTitle(title);
}

ECode CActionBarContextView::SetSubtitle(
    /* [in] */ ICharSequence* subtitle)
{
    return ActionBarContextView::SetSubtitle(subtitle);
}

ECode CActionBarContextView::GetTitle(
    /* [out] */ ICharSequence** title)
{
    assert(title != NULL);
    AutoPtr<ICharSequence> t = ActionBarContextView::GetTitle();
    *title = t;
    INTERFACE_ADDREF(*title);

    return NOERROR;
}

ECode CActionBarContextView::GetSubtitle(
    /* [out] */ ICharSequence** subTitle)
{
    assert(subTitle != NULL);
    AutoPtr<ICharSequence> t = ActionBarContextView::GetSubtitle();
    *subTitle = t;
    INTERFACE_ADDREF(*subTitle);

    return NOERROR;
}

ECode CActionBarContextView::InitForMode(
    /* [in] */ IActionMode* mode)
{
    return ActionBarContextView::InitForMode(mode);
}

ECode CActionBarContextView::CloseMode()
{
    return ActionBarContextView::CloseMode();
}

ECode CActionBarContextView::KillMode()
{
    return ActionBarContextView::KillMode();
}

ECode CActionBarContextView::SetTitleOptional(
    /* [in] */ Boolean titleOptional)
{
    return ActionBarContextView::SetTitleOptional(titleOptional);
}

ECode CActionBarContextView::IsTitleOptional(
    /* [out] */ Boolean* res)
{
    assert(res != NULL);
    *res = ActionBarContextView::IsTitleOptional();
    return NOERROR;
}

ECode CActionBarContextView::SetSplitActionBar(
    /* [in] */ Boolean split)
{
    return ActionBarContextView::SetSplitActionBar(split);
}

ECode CActionBarContextView::SetSplitWhenNarrow(
    /* [in] */ Boolean splitWhenNarrow)
{
    return ActionBarContextView::SetSplitWhenNarrow(splitWhenNarrow);
}

ECode CActionBarContextView::SetContentHeight(
    /* [in] */ Int32 height)
{
    return ActionBarContextView::SetContentHeight(height);
}

ECode CActionBarContextView::GetContentHeight(
    /* [out] */ Int32* height)
{
    assert(height != NULL);
    *height = ActionBarContextView::GetContentHeight();
    return NOERROR;
}

ECode CActionBarContextView::SetSplitView(
    /* [in] */ IActionBarContainer* splitView)
{
    return ActionBarContextView::SetSplitView(splitView);
}

ECode CActionBarContextView::GetAnimatedVisibility(
    /* [out] */ Int32* visibility)
{
    assert(visibility != NULL);
    *visibility = ActionBarContextView::GetAnimatedVisibility();
    return NOERROR;
}

ECode CActionBarContextView::AnimateToVisibility(
    /* [in] */ Int32 visibility)
{
    return ActionBarContextView::AnimateToVisibility(visibility);
}

ECode CActionBarContextView::ShowOverflowMenu(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = ActionBarContextView::ShowOverflowMenu();
    return NOERROR;
}

ECode CActionBarContextView::PostShowOverflowMenu()
{
    return ActionBarContextView::PostShowOverflowMenu();
}

ECode CActionBarContextView::HideOverflowMenu(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = ActionBarContextView::HideOverflowMenu();
    return NOERROR;
}

ECode CActionBarContextView::IsOverflowMenuShowing(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = ActionBarContextView::IsOverflowMenuShowing();
    return NOERROR;
}

ECode CActionBarContextView::IsOverflowReserved(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = ActionBarContextView::IsOverflowReserved();
    return NOERROR;
}

ECode CActionBarContextView::DismissPopupMenus()
{
    return ActionBarContextView::DismissPopupMenus();
}

ECode CActionBarContextView::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    return ActionBarContextView::OnAnimationStart(animation);
}

ECode CActionBarContextView::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    return ActionBarContextView::OnAnimationEnd(animation);
}

ECode CActionBarContextView::OnAnimationCancel(
    /* [in] */ IAnimator* animation)
{
    return ActionBarContextView::OnAnimationCancel(animation);
}

ECode CActionBarContextView::OnAnimationRepeat(
    /* [in] */ IAnimator* animation)
{
    return ActionBarContextView::OnAnimationRepeat(animation);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
