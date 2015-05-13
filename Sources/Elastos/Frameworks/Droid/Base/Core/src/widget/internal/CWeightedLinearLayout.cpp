#include "ext/frameworkext.h"
#include "widget/internal/CWeightedLinearLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {


IVIEW_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IVIEWGROUP_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IVIEWPARENT_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IVIEWMANAGER_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)
ILINEARLAYOUT_METHODS_IMPL(CWeightedLinearLayout, WeightedLinearLayout)

PInterface CWeightedLinearLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return CWeightedLinearLayout::Probe(riid);
}

ECode CWeightedLinearLayout::constructor(
    /* [in] */ IContext* context)
{
    return WeightedLinearLayout::Init(context);
}

ECode CWeightedLinearLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return WeightedLinearLayout::Init(context, attrs);
}

} // namespace Internal
} // namespace Widget
} // namespace Droid
} // namespace Elastos
