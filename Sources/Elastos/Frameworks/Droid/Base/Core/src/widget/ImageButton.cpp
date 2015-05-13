
#include "ext/frameworkext.h"
#include "widget/ImageButton.h"

using Elastos::Droid::Widget::ImageView;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

    const String ImageButton::IMAGEBUTTON_NAME = String("ImageButton");

ImageButton::ImageButton()
{}

ImageButton::ImageButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context, attrs, defStyle);
}

ECode ImageButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(ImageView::Init(context, attrs, defStyle));
    SetFocusable(TRUE);
    return NOERROR;
}

Boolean ImageButton::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    return FALSE;
}

//@Override
ECode ImageButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ImageView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(IMAGEBUTTON_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

//@Override
ECode ImageButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ImageView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(IMAGEBUTTON_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
