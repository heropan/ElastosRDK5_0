#include "elastos/droid/widget/CZoomControls.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

ILINEARLAYOUT_METHODS_IMPL(CZoomControls, ZoomControls)
IVIEW_METHODS_IMPL(CZoomControls, ZoomControls)
IVIEWGROUP_METHODS_IMPL(CZoomControls, ZoomControls)
IVIEWPARENT_METHODS_IMPL(CZoomControls, ZoomControls)
IVIEWMANAGER_METHODS_IMPL(CZoomControls, ZoomControls)
IDRAWABLECALLBACK_METHODS_IMPL(CZoomControls, ZoomControls)
IKEYEVENTCALLBACK_METHODS_IMPL(CZoomControls, ZoomControls)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CZoomControls, ZoomControls)

PInterface CZoomControls::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CZoomControls::Probe(riid);
}

ECode CZoomControls::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ZoomControls::Init(context, attrs);
}

ECode CZoomControls::SetOnZoomInClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return ZoomControls::SetOnZoomInClickListener(listener);
}

ECode CZoomControls::SetOnZoomOutClickListener(
    /* [in] */ IViewOnClickListener* listener)
{
    return ZoomControls::SetOnZoomOutClickListener(listener);
}

/*
 * Sets how fast you get zoom events when the user holds down the
 * zoom in/out buttons.
 */
ECode CZoomControls::SetZoomSpeed(
    /* [in] */ Int64 speed)
{
    return ZoomControls::SetZoomSpeed(speed);
}

ECode CZoomControls::Show()
{
    return ZoomControls::Show();
}

ECode CZoomControls::Hide()
{
    return ZoomControls::Hide();
}

ECode CZoomControls::SetIsZoomInEnabled(
    /* [in] */ Boolean isEnabled)
{
    return ZoomControls::SetIsZoomInEnabled(isEnabled);
}

ECode CZoomControls::SetIsZoomOutEnabled(
    /* [in] */ Boolean isEnabled)
{
    return ZoomControls::SetIsZoomOutEnabled(isEnabled);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos