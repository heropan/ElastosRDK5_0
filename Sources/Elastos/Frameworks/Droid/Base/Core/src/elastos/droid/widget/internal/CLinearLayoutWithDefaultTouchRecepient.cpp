#include "CLinearLayoutWithDefaultTouchRecepient.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

ILINEARLAYOUT_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IVIEW_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IVIEWGROUP_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IVIEWPARENT_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IVIEWMANAGER_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IDRAWABLECALLBACK_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IKEYEVENTCALLBACK_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CLinearLayoutWithDefaultTouchRecepient, LinearLayoutWithDefaultTouchRecepient)

PInterface CLinearLayoutWithDefaultTouchRecepient::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CLinearLayoutWithDefaultTouchRecepient::Probe(riid);
}

ECode CLinearLayoutWithDefaultTouchRecepient::constructor(
    /* [in] */ IContext* ctx)
{
    return LinearLayoutWithDefaultTouchRecepient::Init(ctx);
}

ECode CLinearLayoutWithDefaultTouchRecepient::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayoutWithDefaultTouchRecepient::Init(ctx, attrs);
}

ECode CLinearLayoutWithDefaultTouchRecepient::SetDefaultTouchRecepient(
    /* [in] */ IView* defaultTouchRecepient)
{
    return LinearLayoutWithDefaultTouchRecepient::SetDefaultTouchRecepient(defaultTouchRecepient);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
