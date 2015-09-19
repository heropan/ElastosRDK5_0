#include "widget/CSpace.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CSpace, Space)
IDRAWABLECALLBACK_METHODS_IMPL(CSpace, Space)
IKEYEVENTCALLBACK_METHODS_IMPL(CSpace, Space)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSpace, Space)

PInterface CSpace::Probe(
    /* [in] */ REIID riid)
{
    if (riid == Elastos::Droid::View::EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CSpace::Probe(riid);
}

ECode CSpace::constructor(
    /* [in] */ IContext* context)
{
    return Space::Init(context);
}

ECode CSpace::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Space::Init(context, attrs);
}

ECode CSpace::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Space::Init(context, attrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
