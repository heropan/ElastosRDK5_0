#include "widget/CRecycleListView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CRecycleListView, RecycleListView)
IVIEWGROUP_METHODS_IMPL(CRecycleListView, RecycleListView)
IVIEWPARENT_METHODS_IMPL(CRecycleListView, RecycleListView)
IVIEWMANAGER_METHODS_IMPL(CRecycleListView, RecycleListView)
IDRAWABLECALLBACK_METHODS_IMPL(CRecycleListView, RecycleListView)
IKEYEVENTCALLBACK_METHODS_IMPL(CRecycleListView, RecycleListView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CRecycleListView, RecycleListView)
IADAPTERVIEW_METHODS_IMPL(CRecycleListView, RecycleListView)
IABSLISTVIEW_METHODS_IMPL(CRecycleListView, RecycleListView)
IABSLISTVIEW_INTERFACE_METHODS_IMPL(CRecycleListView, RecycleListView)
ILISTVIEW_METHODS_IMPL(CRecycleListView, RecycleListView)

PInterface CRecycleListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CRecycleListView::Probe(riid);
}

ECode CRecycleListView::constructor(
    /* [in] */ IContext* ctx)
{
    return RecycleListView::Init(ctx);
}

ECode CRecycleListView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return RecycleListView::Init(ctx, attrs);
}

ECode CRecycleListView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return RecycleListView::Init(ctx, attrs, defStyle);
}

ECode CRecycleListView::GetRecycleOnMeasure(
    /* [out] */ Boolean* recycleOnMeasure)
{
    VALIDATE_NOT_NULL(recycleOnMeasure)
    *recycleOnMeasure = RecycleListView::GetRecycleOnMeasure();
    return NOERROR;
}

ECode CRecycleListView::SetRecycleOnMeasure(
    /* [in] */ Boolean recycleOnMeasure)
{
    return RecycleListView::SetRecycleOnMeasure(recycleOnMeasure);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
