#include "elastos/droid/widget/internal/CSizeAdaptiveLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IVIEWGROUP_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IVIEWPARENT_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IVIEWMANAGER_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSizeAdaptiveLayout, SizeAdaptiveLayout)

PInterface CSizeAdaptiveLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CSizeAdaptiveLayout::Probe(riid);
}

ECode CSizeAdaptiveLayout::constructor(
    /* [in] */ IContext* context)
{
    return SizeAdaptiveLayout::Init(context);
}

ECode CSizeAdaptiveLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SizeAdaptiveLayout::Init(context, attrs);
}

ECode CSizeAdaptiveLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SizeAdaptiveLayout::Init(context, attrs, defStyle);
}

ECode CSizeAdaptiveLayout::GetTransitionAnimation(
    /* [out] */ IAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    AutoPtr<IAnimator> tmp = SizeAdaptiveLayout::GetTransitionAnimation();
    *animator = tmp;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

ECode CSizeAdaptiveLayout::GetModestyPanel(
    /* [out] */ IView** panel)
{
    VALIDATE_NOT_NULL(panel);
    AutoPtr<IView> tmp = SizeAdaptiveLayout::GetModestyPanel();
    *panel = tmp;
    REFCOUNT_ADD(*panel);
    return NOERROR;
}

ECode CSizeAdaptiveLayout::OnAttachedToWindow()
{
    return SizeAdaptiveLayout::OnAttachedToWindow();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
