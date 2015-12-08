
#include "elastos/droid/widget/Button.h"

using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(Button, TextView, IButton);

Button::Button()
{
}

ECode Button::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    return TextView::constructor(context, attrs, defStyleAttr, defStyleRes);
}

ECode Button::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> txt;
    CString::New(String("Button"), (ICharSequence**)&txt);
    return IAccessibilityRecord::Probe(event)->SetClassName(txt);
}

ECode Button::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> txt;
    CString::New(String("Button"), (ICharSequence**)&txt);
    return info->SetClassName(txt);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
