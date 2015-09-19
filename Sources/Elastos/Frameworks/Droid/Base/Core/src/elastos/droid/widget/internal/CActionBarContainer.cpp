#include "widget/internal/CActionBarContainer.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IVIEWGROUP_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IVIEWPARENT_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IVIEWMANAGER_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IDRAWABLECALLBACK_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IKEYEVENTCALLBACK_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CActionBarContainer, ActionBarContainer)
IFRAMELAYOUT_METHODS_IMPL(CActionBarContainer, ActionBarContainer)

PInterface CActionBarContainer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CActionBarContainer::Probe(riid);
}

ECode CActionBarContainer::constructor(
    /* [in] */ IContext* context)
{
    return ActionBarContainer::Init(context);
}

ECode CActionBarContainer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ActionBarContainer::Init(context, attrs);
}

ECode CActionBarContainer::SetPrimaryBackground(
    /* [in] */ IDrawable* bg)
{
    return ActionBarContainer::SetPrimaryBackground(bg);
}

ECode CActionBarContainer::SetStackedBackground(
    /* [in] */ IDrawable* bg)
{
    return ActionBarContainer::SetStackedBackground(bg);
}

ECode CActionBarContainer::SetSplitBackground(
    /* [in] */ IDrawable* bg)
{
    return ActionBarContainer::SetSplitBackground(bg);
}

ECode CActionBarContainer::SetTransitioning(
    /* [in] */ Boolean isTransitioning)
{
    return ActionBarContainer::SetTransitioning(isTransitioning);
}

ECode CActionBarContainer::SetTabContainer(
    /* [in] */ IScrollingTabContainerView* tabView)
{
    return ActionBarContainer::SetTabContainer(tabView);
}

ECode CActionBarContainer::GetTabContainer(
    /* [out] */ IView** container)
{

    VALIDATE_NOT_NULL(container);
    *container = ActionBarContainer::GetTabContainer();
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
