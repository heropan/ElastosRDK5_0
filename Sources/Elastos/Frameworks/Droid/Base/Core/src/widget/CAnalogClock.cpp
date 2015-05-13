#include "CAnalogClock.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CAnalogClock, AnalogClock)

IDRAWABLECALLBACK_METHODS_IMPL(CAnalogClock, AnalogClock)

IKEYEVENTCALLBACK_METHODS_IMPL(CAnalogClock, AnalogClock)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CAnalogClock, AnalogClock)

PInterface CAnalogClock::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CAnalogClock::Probe(riid);
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pCtx)
{
    return AnalogClock::Init(pCtx);
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pCtx,
    /* [in] */ IAttributeSet * pAttrs)
{
    return AnalogClock::Init(pCtx, pAttrs);
}

ECode CAnalogClock::constructor(
    /* [in] */ IContext * pContext,
    /* [in] */ IAttributeSet * pAttrs,
    /* [in] */ Int32 defStyle)
{
    return AnalogClock::Init(pContext, pAttrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
