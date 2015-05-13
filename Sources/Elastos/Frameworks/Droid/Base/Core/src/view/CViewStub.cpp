
#include "view/CViewStub.h"

namespace Elastos {
namespace Droid {
namespace View {


IVIEW_METHODS_IMPL(CViewStub, ViewStub);

IDRAWABLECALLBACK_METHODS_IMPL(CViewStub, ViewStub);

IKEYEVENTCALLBACK_METHODS_IMPL(CViewStub, ViewStub);

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CViewStub, ViewStub);

ECode CViewStub::constructor(
    /* [in] */ IContext* context)
{
    return ViewStub::InitViewStub(context);
}

ECode CViewStub::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutResource)
{
    return ViewStub::InitViewStub(context, layoutResource);
}

ECode CViewStub::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewStub::InitViewStub(context, attrs);
}

ECode CViewStub::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ViewStub::InitViewStub(context, attrs, defStyle);
}

PInterface CViewStub::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CViewStub::Probe(riid);
}

ECode CViewStub::GetInflatedId(
    /* [out] */ Int32* id)
{
    return ViewStub::GetInflatedId(id);
}

ECode CViewStub::SetInflatedId(
    /* [in] */ Int32 inflatedId)
{
    return ViewStub::SetInflatedId(inflatedId);
}

ECode CViewStub::GetLayoutResource(
    /* [out] */ Int32* resource)
{
    return ViewStub::GetLayoutResource(resource);
}

ECode CViewStub::SetLayoutResource(
    /* [in] */ Int32 layoutResource)
{
    return ViewStub::SetLayoutResource(layoutResource);
}

ECode CViewStub::SetLayoutInflater(
    /* [in] */ ILayoutInflater* inflater)
{
    return ViewStub::SetLayoutInflater(inflater);
}

ECode CViewStub::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    return ViewStub::GetLayoutInflater(inflater);
}

ECode CViewStub::Inflate(
    /* [out] */ IView** retView)
{
    return ViewStub::Inflate(retView);
}

ECode CViewStub::SetOnInflateListener(
    /* [in] */ IOnInflateListener* inflateListener)
{
    return ViewStub::SetOnInflateListener(inflateListener);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
