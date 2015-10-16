#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CImageSwitcher.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IFRAMELAYOUT_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IVIEWGROUP_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IVIEWPARENT_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IVIEWMANAGER_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IDRAWABLECALLBACK_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IKEYEVENTCALLBACK_METHODS_IMPL(CImageSwitcher, ImageSwitcher)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CImageSwitcher, ImageSwitcher)

PInterface CImageSwitcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CImageSwitcher::Probe(riid);
}

ECode CImageSwitcher::constructor(
    /* [in] */ IContext* context)
{
    return ImageSwitcher::Init(context);
}

ECode CImageSwitcher::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ImageSwitcher::Init(context, attrs);
}

ECode CImageSwitcher::SetImageResource(
    /* [in] */ Int32 resid)
{
    return ImageSwitcher::SetImageResource(resid);
}

ECode CImageSwitcher::SetImageURI(
    /* [in] */ IUri* uri)
{
    return ImageSwitcher::SetImageURI(uri);
}

ECode CImageSwitcher::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    return ImageSwitcher::SetImageDrawable(drawable);
}

ECode CImageSwitcher::SetDisplayedChild(
    /* [in] */ Int32 whichChild)
{
    return ImageSwitcher::SetDisplayedChild(whichChild);
}

ECode CImageSwitcher::GetDisplayedChild(
    /* [out] */ Int32* child)
{
    return ImageSwitcher::GetDisplayedChild(child);
}

ECode CImageSwitcher::ShowNext()
{
    return ImageSwitcher::ShowNext();
}

ECode CImageSwitcher::ShowPrevious()
{
    return ImageSwitcher::ShowPrevious();
}

ECode CImageSwitcher::GetCurrentView(
    /* [out] */ IView** view)
{
    return ImageSwitcher::GetCurrentView(view);
}

ECode CImageSwitcher::GetInAnimation(
    /* [out] */ IAnimation** animation)
{
    return ImageSwitcher::GetInAnimation(animation);
}

ECode CImageSwitcher::SetInAnimation(
    /* [in] */ IAnimation* inAnimation)
{
    return ImageSwitcher::SetInAnimation(inAnimation);
}

ECode CImageSwitcher::GetOutAnimation(
    /* [out] */ IAnimation** animation)
{
    return ImageSwitcher::GetOutAnimation(animation);
}

ECode CImageSwitcher::SetOutAnimation(
    /* [in] */ IAnimation* outAnimation)
{
    return ImageSwitcher::SetOutAnimation(outAnimation);
}

ECode CImageSwitcher::SetInAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return ImageSwitcher::SetInAnimation(context, resourceID);
}

ECode CImageSwitcher::SetOutAnimation(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resourceID)
{
    return ImageSwitcher::SetOutAnimation(context, resourceID);
}

ECode CImageSwitcher::GetAnimateFirstView(
    /* [out] */ Boolean* animate)
{
    return ImageSwitcher::GetAnimateFirstView(animate);
}

ECode CImageSwitcher::SetAnimateFirstView(
    /* [in] */ Boolean animate)
{
    return ImageSwitcher::SetAnimateFirstView(animate);
}

ECode CImageSwitcher::GetNextView(
    /* [out] */ IView** view)
{
    return ImageSwitcher::GetNextView(view);
}

ECode CImageSwitcher::SetFactory(
    /* [in] */ IViewFactory* factory)
{
    return ImageSwitcher::SetFactory(factory);
}

ECode CImageSwitcher::Reset()
{
    return ImageSwitcher::Reset();
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
