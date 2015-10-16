#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CDateTimeView.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CDateTimeView, DateTimeView)
ITEXTVIEW_METHODS_IMPL(CDateTimeView, DateTimeView);
IDRAWABLECALLBACK_METHODS_IMPL(CDateTimeView, DateTimeView)
IKEYEVENTCALLBACK_METHODS_IMPL(CDateTimeView, DateTimeView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CDateTimeView, DateTimeView)

CARAPI_(PInterface) CDateTimeView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CDateTimeView::Probe(riid);
}

ECode CDateTimeView::constructor(
        /* [in] */ IContext* context)
{
    return DateTimeView::Init(context);
}

ECode CDateTimeView::constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    return DateTimeView::Init(context, attrs);
}

ECode CDateTimeView::SetTime(
        /* [in] */ Int64 time)
{
    return DateTimeView::SetTime(time);
}

ECode CDateTimeView::Update()
{
    return DateTimeView::Update();
}

ECode CDateTimeView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = DateTimeView::OnPreDraw();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
