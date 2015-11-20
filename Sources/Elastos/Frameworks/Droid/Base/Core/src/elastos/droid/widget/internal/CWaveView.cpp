#include "CWaveView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CWaveView, WaveView)

IDRAWABLECALLBACK_METHODS_IMPL(CWaveView, WaveView)

IKEYEVENTCALLBACK_METHODS_IMPL(CWaveView, WaveView)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CWaveView, WaveView)

ECode CWaveView::constructor(
    /* [in] */ IContext* ctx)
{
    return WaveView::Init(ctx);
}

ECode CWaveView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return WaveView::Init(ctx, attrs);
}

PInterface CWaveView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CWaveView::Probe(riid);
}

ECode CWaveView::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    return WaveView::OnAnimationUpdate(animation);
}

ECode CWaveView::SetOnTriggerListener(
    /* [in] */ IWaveViewOnTriggerListener* listener)
{
    return WaveView::SetOnTriggerListener(listener);
}

ECode CWaveView::Reset()
{
    return WaveView::Reset();
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
