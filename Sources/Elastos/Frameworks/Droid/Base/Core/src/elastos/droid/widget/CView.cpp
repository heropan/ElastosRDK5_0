
#include "elastos/droid/widget/CView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CView, Elastos::Droid::View::View)
IDRAWABLECALLBACK_METHODS_IMPL(CView, Elastos::Droid::View::View)
IKEYEVENTCALLBACK_METHODS_IMPL(CView, Elastos::Droid::View::View)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CView, Elastos::Droid::View::View)

PInterface CView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == Elastos::Droid::View::EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CView::Probe(riid);
}

ECode CView::constructor(
    /* [in] */ IContext* context)
{
    return Elastos::Droid::View::View::Init(context);
}

ECode CView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Elastos::Droid::View::View::Init(context, attrs);
}

ECode CView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return Elastos::Droid::View::View::Init(context, attrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
