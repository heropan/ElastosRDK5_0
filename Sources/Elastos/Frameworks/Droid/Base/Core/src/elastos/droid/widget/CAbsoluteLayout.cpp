
#include "elastos/droid/widget/CAbsoluteLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IVIEWGROUP_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IVIEWPARENT_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IVIEWMANAGER_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CAbsoluteLayout, AbsoluteLayout)


PInterface CAbsoluteLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CAbsoluteLayout::Probe(riid);
}


ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext* context)
{
    return AbsoluteLayout::Init(context);
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AbsoluteLayout::Init(context, attrs);
}

ECode CAbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AbsoluteLayout::Init(context, attrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
