#include "widget/CAdapterViewAnimator.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IVIEWGROUP_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IVIEWPARENT_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IVIEWMANAGER_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IDRAWABLECALLBACK_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IKEYEVENTCALLBACK_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)
IADAPTERVIEW_METHODS_IMPL(CAdapterViewAnimator, AdapterViewAnimator)

PInterface CAdapterViewAnimator::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CAdapterViewAnimator::Probe(riid);
}

ECode CAdapterViewAnimator::constructor(
    /* [in] */ IContext* context)
{
    return AdapterViewAnimator::Init(context);
}

ECode CAdapterViewAnimator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AdapterViewAnimator::Init(context, attrs);
}

ECode CAdapterViewAnimator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AdapterViewAnimator::Init(context, attrs, defStyle);
}

ECode CAdapterViewAnimator::SetDisplayedChild(
    /* [in] */ Int32 whichChild)
{
    return AdapterViewAnimator::SetDisplayedChild(whichChild);
}

ECode CAdapterViewAnimator::GetDisplayedChild(
    /* [out] */ Int32* child)
{
    VALIDATE_NOT_NULL(child);
    *child = AdapterViewAnimator::GetDisplayedChild();
    return NOERROR;
}

ECode CAdapterViewAnimator::ShowNext()
{
    return AdapterViewAnimator::ShowNext();
}

ECode CAdapterViewAnimator::ShowPrevious()
{
    return AdapterViewAnimator::ShowPrevious();
}

ECode CAdapterViewAnimator::GetCurrentView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = AdapterViewAnimator::GetCurrentView();
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAdapterViewAnimator::GetInAnimation(
    /* [out] */ IObjectAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    AutoPtr<IObjectAnimator> oa = AdapterViewAnimator::GetInAnimation();
    *animator = oa;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

ECode CAdapterViewAnimator::SetInAnimation(
    /* [in] */ IObjectAnimator* inAnimation)
{
    return AdapterViewAnimator::SetInAnimation(inAnimation);
}

ECode CAdapterViewAnimator::GetOutAnimation(
    /* [out] */ IObjectAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    AutoPtr<IObjectAnimator> oa = AdapterViewAnimator::GetOutAnimation();
    *animator = oa;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

ECode CAdapterViewAnimator::SetOutAnimation(
    /* [in] */ IObjectAnimator* outAnimation)
{
    return AdapterViewAnimator::SetOutAnimation(outAnimation);
}

ECode CAdapterViewAnimator::SetInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
     return AdapterViewAnimator::SetInAnimation(context, resourceID);
}

ECode CAdapterViewAnimator::SetOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return AdapterViewAnimator::SetOutAnimation(context, resourceID);
}

ECode CAdapterViewAnimator::SetAnimateFirstView(
    /* [in] */ Boolean animate)
{
    return AdapterViewAnimator::SetAnimateFirstView(animate);
}

ECode CAdapterViewAnimator::SetRemoteViewsOnClickHandler(
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    return AdapterViewAnimator::SetRemoteViewsOnClickHandler(handler);
}

ECode CAdapterViewAnimator::SetRemoteViewsAdapter(
    /* [in] */ IIntent* intent)
{
    return AdapterViewAnimator::SetRemoteViewsAdapter(intent);
}

ECode CAdapterViewAnimator::Advance()
{
    return AdapterViewAnimator::Advance();
}

ECode CAdapterViewAnimator::FyiWillBeAdvancedByHostKThx()
{
    return AdapterViewAnimator::FyiWillBeAdvancedByHostKThx();
}

ECode CAdapterViewAnimator::OnRemoteAdapterConnected(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = AdapterViewAnimator::OnRemoteAdapterConnected();
    return NOERROR;
}

ECode CAdapterViewAnimator::OnRemoteAdapterDisconnected()
{
    return AdapterViewAnimator::OnRemoteAdapterDisconnected();
}

ECode CAdapterViewAnimator::DeferNotifyDataSetChanged()
{
    return AdapterViewAnimator::DeferNotifyDataSetChanged();
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
