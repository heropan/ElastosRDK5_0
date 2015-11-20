
#include "elastos/droid/widget/CheckBox.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

CheckBox::CheckBox()
{}

CheckBox::CheckBox(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
{}

ECode CheckBox::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return CompoundButton::Init(context, attrs, defStyle);
}

//@Override
ECode CheckBox::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FAIL_RETURN(CompoundButton::OnInitializeAccessibilityEvent(event));
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("CheckBox"), (ICharSequence**)&txt);
    return event->SetClassName(txt);
}

//@Override
ECode CheckBox::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FAIL_RETURN(CompoundButton::OnInitializeAccessibilityNodeInfo(info));
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("CheckBox"), (ICharSequence**)&txt);
    return info->SetClassName(txt);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
