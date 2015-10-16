#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CDigitalClock.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CDigitalClock, DigitalClock)
IDRAWABLECALLBACK_METHODS_IMPL(CDigitalClock, DigitalClock)
IKEYEVENTCALLBACK_METHODS_IMPL(CDigitalClock, DigitalClock)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CDigitalClock, DigitalClock)
ITEXTVIEW_METHODS_IMPL(CDigitalClock, DigitalClock)

PInterface CDigitalClock::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CDigitalClock::Probe(riid);
}

ECode CDigitalClock::constructor(
    /* [in] */ IContext* context)
{
    return DigitalClock::Init(context);
}

ECode CDigitalClock::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return DigitalClock::Init(context, attrs);
}

ECode CDigitalClock::OnPreDraw(
    /* [OUT] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = DigitalClock::OnPreDraw();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos