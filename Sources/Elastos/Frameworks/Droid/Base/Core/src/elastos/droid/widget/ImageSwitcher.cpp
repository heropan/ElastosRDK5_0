#include "ext/frameworkext.h"
#include "widget/ImageSwitcher.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

const String ImageSwitcher::IMAGESWITCHER_NAME = String("ImageSwitcher");

ImageSwitcher::ImageSwitcher(
    /* [in] */ IContext* context)
{
    Init(context);
}

ImageSwitcher::ImageSwitcher(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ImageSwitcher::ImageSwitcher()
{

}

ECode ImageSwitcher::Init(
        /* [in] */ IContext* context)
{
    ViewSwitcher::Init(context);
    return NOERROR;
}

ECode ImageSwitcher::Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs)
{
    ViewSwitcher::Init(context, attrs);
    return NOERROR;
}

ECode ImageSwitcher::SetImageResource(
    /* [in] */ Int32 resid)
{
    AutoPtr<IView> view;
    GetNextView((IView**)&view);
    AutoPtr<IImageView> image = IImageView::Probe(view);
    image->SetImageResource(resid);
    ShowNext();
    return NOERROR;
}

ECode ImageSwitcher::SetImageURI(
    /* [in] */ IUri* uri)
{
    AutoPtr<IView> view;
    GetNextView((IView**)&view);
    AutoPtr<IImageView> image = IImageView::Probe(view);
    image->SetImageURI(uri);
    ShowNext();
    return NOERROR;
}

ECode ImageSwitcher::SetImageDrawable(
    /* [in] */ IDrawable* drawable)
{
    AutoPtr<IView> view;
    GetNextView((IView**)&view);
    AutoPtr<IImageView> image = IImageView::Probe(view);
    image->SetImageDrawable(drawable);
    ShowNext();
    return NOERROR;
}

ECode ImageSwitcher::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewSwitcher::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(IMAGESWITCHER_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode ImageSwitcher::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewSwitcher::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(IMAGESWITCHER_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos