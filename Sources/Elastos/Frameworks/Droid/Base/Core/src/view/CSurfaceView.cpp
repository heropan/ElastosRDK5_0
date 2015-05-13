
#include "view/CSurfaceView.h"

namespace Elastos {
namespace Droid {
namespace View {


IVIEW_METHODS_IMPL(CSurfaceView, SurfaceView);
IDRAWABLECALLBACK_METHODS_IMPL(CSurfaceView, SurfaceView);
IKEYEVENTCALLBACK_METHODS_IMPL(CSurfaceView, SurfaceView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSurfaceView, SurfaceView);

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context)
{
    return SurfaceView::Init(context);
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SurfaceView::Init(context, attrs);
}

ECode CSurfaceView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return SurfaceView::Init(context, attrs, defStyle);
}

PInterface CSurfaceView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSurfaceView::Probe(riid);
}

ECode CSurfaceView::GetHolder(
    /* [out] */ ISurfaceHolder** holder)
{
    assert(holder != NULL);
    AutoPtr<ISurfaceHolder> _holder = SurfaceView::GetHolder();
    *holder = _holder;
    INTERFACE_ADDREF(*holder );
    return NOERROR;
}

ECode CSurfaceView::SetZOrderMediaOverlay(
    /* [in] */ Boolean isMediaOverlay)
{
    return SurfaceView::SetZOrderMediaOverlay(isMediaOverlay);
}

ECode CSurfaceView::SetZOrderOnTop(
    /* [in] */ Boolean onTop)
{
    return SurfaceView::SetZOrderOnTop(onTop);
}

ECode CSurfaceView::SetSecure(
    /* [in] */ Boolean isSecure)
{
    return SurfaceView::SetSecure(isSecure);
}

ECode CSurfaceView::SetWindowType(
    /* [in] */ Int32 type)
{
    return SurfaceView::SetWindowType(type);
}

ECode CSurfaceView::IsFixedSize(
    /* [out] */ Boolean* size)
{
    assert(size != NULL);
    *size = SurfaceView::IsFixedSize();

    return NOERROR;
}

ECode CSurfaceView::SetCreateInputConnectionDelegate(
    /* [in] */ IView* view)
{
    return SurfaceView::SetCreateInputConnectionDelegate(view);
}

ECode CSurfaceView::SetKeyEventCallbackDelegate(
    /* [in] */IKeyEventCallback* cb)
{
    return SurfaceView::SetKeyEventCallbackDelegate(cb);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
