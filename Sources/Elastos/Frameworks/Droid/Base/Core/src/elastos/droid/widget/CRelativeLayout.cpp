
#include "widget/CRelativeLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IVIEWGROUP_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IVIEWPARENT_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IVIEWMANAGER_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CRelativeLayout, RelativeLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CRelativeLayout, RelativeLayout)


PInterface CRelativeLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CRelativeLayout::Probe(riid);
}

ECode CRelativeLayout::constructor(
    /* [in] */ IContext* context)
{
    return RelativeLayout::Init(context);
}

ECode CRelativeLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RelativeLayout::Init(context, attrs);
}

ECode CRelativeLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return RelativeLayout::Init(context, attrs, defStyle);
}

ECode CRelativeLayout::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return RelativeLayout::SetIgnoreGravity(viewId);
}

ECode CRelativeLayout::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = RelativeLayout::GetGravity();
    return NOERROR;
}

ECode CRelativeLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return RelativeLayout::SetGravity(gravity);
}

ECode CRelativeLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return RelativeLayout::SetHorizontalGravity(horizontalGravity);
}

ECode CRelativeLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return RelativeLayout::SetVerticalGravity(verticalGravity);
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
