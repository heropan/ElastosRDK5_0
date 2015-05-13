#include "widget/internal/CPointerLocationView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CPointerLocationView, PointerLocationView)
IDRAWABLECALLBACK_METHODS_IMPL(CPointerLocationView, PointerLocationView)
IKEYEVENTCALLBACK_METHODS_IMPL(CPointerLocationView, PointerLocationView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CPointerLocationView, PointerLocationView)

CPointerLocationView::CPointerLocationView()
{}

ECode CPointerLocationView::constructor(
        /* [in] */ IContext* ctx)
{
    return CPointerLocationView::Init(ctx);
}

PInterface CPointerLocationView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CPointerLocationView::Probe(riid);
}

ECode CPointerLocationView::SetPrintCoords(
    /* [in] */ Boolean state)
{
    return CPointerLocationView::SetPrintCoords(state);
}

ECode CPointerLocationView::AddPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    return CPointerLocationView::AddPointerEvent(event);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos