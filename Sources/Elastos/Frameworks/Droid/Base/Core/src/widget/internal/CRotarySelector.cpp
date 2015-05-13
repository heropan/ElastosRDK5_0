#include "widget/internal/CRotarySelector.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

IVIEW_METHODS_IMPL(CRotarySelector, RotarySelector)
IDRAWABLECALLBACK_METHODS_IMPL(CRotarySelector, RotarySelector)
IKEYEVENTCALLBACK_METHODS_IMPL(CRotarySelector, RotarySelector)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CRotarySelector, RotarySelector)

CRotarySelector::CRotarySelector()
{}

ECode CRotarySelector::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RotarySelector::Init(context, attrs);
}

ECode CRotarySelector::constructor(
    /* [in] */ IContext* context)
{
    return RotarySelector::Init(context);
}

PInterface CRotarySelector::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CRotarySelector::Probe(riid);
}

ECode CRotarySelector::SetLeftHandleResource(
    /* [in] */ Int32 resId)
{
    return RotarySelector::SetLeftHandleResource(resId);
}

ECode CRotarySelector::SetRightHandleResource(
    /* [in] */ Int32 resId)
{
    return RotarySelector::SetRightHandleResource(resId);
}

ECode CRotarySelector::SetOnDialTriggerListener(
    /* [in] */ IOnDialTriggerListener* l)
{
    return RotarySelector::SetOnDialTriggerListener(l);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos