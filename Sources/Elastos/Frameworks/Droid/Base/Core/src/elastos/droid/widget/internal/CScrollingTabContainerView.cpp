#include "elastos/droid/widget/internal/CScrollingTabContainerView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IVIEWGROUP_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IVIEWPARENT_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IVIEWMANAGER_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IFRAMELAYOUT_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IDRAWABLECALLBACK_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IKEYEVENTCALLBACK_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)
IHORIZONTALSCROLLVIEW_METHODS_IMPL(CScrollingTabContainerView, ScrollingTabContainerView)

PInterface CScrollingTabContainerView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CScrollingTabContainerView::Probe(riid);
}

ECode CScrollingTabContainerView::constructor(
    /* [in] */ IContext* context)
{
    ScrollingTabContainerView::Init(context);
    return NOERROR;
}

ECode CScrollingTabContainerView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return ScrollingTabContainerView::OnItemClick(parent, view, position, id);
}

ECode CScrollingTabContainerView::SetAllowCollapse(
    /* [in] */ Boolean allowCollapse)
{
    return ScrollingTabContainerView::SetAllowCollapse(allowCollapse);
}

ECode CScrollingTabContainerView::SetTabSelected(
    /* [in] */ Int32 position)
{
    return ScrollingTabContainerView::SetTabSelected(position);
}

ECode CScrollingTabContainerView::SetContentHeight(
    /* [in] */ Int32 contentHeight)
{
    return ScrollingTabContainerView::SetContentHeight(contentHeight);
}

ECode CScrollingTabContainerView::AnimateToVisibility(
    /* [in] */ Int32 visibility)
{
    return ScrollingTabContainerView::AnimateToVisibility(visibility);
}

ECode CScrollingTabContainerView::AnimateToTab(
    /* [in] */ Int32 position)
{
    return ScrollingTabContainerView::AnimateToTab(position);
}

ECode CScrollingTabContainerView::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Boolean setSelected)
{
    return ScrollingTabContainerView::AddTab(tab, setSelected);
}

ECode CScrollingTabContainerView::AddTab(
    /* [in] */ IActionBarTab* tab,
    /* [in] */ Int32 position,
    /* [in] */ Boolean setSelected)
{
    return ScrollingTabContainerView::AddTab(tab, position, setSelected);
}

ECode CScrollingTabContainerView::UpdateTab(
    /* [in] */ Int32 position)
{
    return ScrollingTabContainerView::UpdateTab(position);
}

ECode CScrollingTabContainerView::RemoveTabAt(
    /* [in] */ Int32 position)
{
    return ScrollingTabContainerView::RemoveTabAt(position);
}

ECode CScrollingTabContainerView::RemoveAllTabs()
{
    return ScrollingTabContainerView::RemoveAllTabs();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
