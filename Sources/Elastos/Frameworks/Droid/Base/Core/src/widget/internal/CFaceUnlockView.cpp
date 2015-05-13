#include "CFaceUnlockView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IVIEWGROUP_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IVIEWPARENT_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IVIEWMANAGER_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IDRAWABLECALLBACK_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IKEYEVENTCALLBACK_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CFaceUnlockView, FaceUnlockView)

PInterface CFaceUnlockView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CFaceUnlockView::Probe(riid);
}

ECode CFaceUnlockView::constructor(
    /* [in] */ IContext* ctx)
{
    return FaceUnlockView::Init(ctx);
}

ECode CFaceUnlockView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return FaceUnlockView::Init(ctx, attrs);
}

ECode CFaceUnlockView::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return FaceUnlockView::SetIgnoreGravity(viewId);
}

ECode CFaceUnlockView::GetGravity(
    /* [out] */ Int32* gravity)
{
    *gravity = FaceUnlockView::GetGravity();
    return NOERROR;
}

ECode CFaceUnlockView::SetGravity(
    /* [in] */ Int32 gravity)
{
    return FaceUnlockView::SetGravity(gravity);
}

ECode CFaceUnlockView::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return FaceUnlockView::SetHorizontalGravity(horizontalGravity);
}

ECode CFaceUnlockView::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return FaceUnlockView::SetVerticalGravity(verticalGravity);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
