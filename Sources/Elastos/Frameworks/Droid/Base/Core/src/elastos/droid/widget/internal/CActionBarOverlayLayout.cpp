#include "elastos/droid/widget/internal/CActionBarOverlayLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IVIEWGROUP_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IVIEWPARENT_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IVIEWMANAGER_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)
IFRAMELAYOUT_METHODS_IMPL(CActionBarOverlayLayout, ActionBarOverlayLayout)

PInterface CActionBarOverlayLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CActionBarOverlayLayout::Probe(riid);
}

ECode CActionBarOverlayLayout::constructor(
    /* [in] */ IContext* context)
{
    return ActionBarOverlayLayout::Init(context);
}

ECode CActionBarOverlayLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ActionBarOverlayLayout::Init(context, attrs);
}

ECode CActionBarOverlayLayout::SetActionBar(
    /* [in] */ IActionBarImpl* impl)
{
    return ActionBarOverlayLayout::SetActionBar(impl);
}

ECode CActionBarOverlayLayout::SetShowingForActionMode(
    /* [in] */ Boolean showing)
{
    return ActionBarOverlayLayout::SetShowingForActionMode(showing);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos