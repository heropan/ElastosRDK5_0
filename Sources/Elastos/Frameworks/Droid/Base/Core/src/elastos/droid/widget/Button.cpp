
#include "widget/Button.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::CPathClassLoader;
using Elastos::Core::IClassLoader;

namespace Elastos{
namespace Droid{
namespace Widget{


Button::Button()
{
}

Button::Button(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : TextView(context, attrs, defStyle)
{
}

ECode Button::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextView::Init(context, attrs, defStyle);
}

ECode Button::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("Button"), (ICharSequence**)&txt);
    return event->SetClassName(txt);
}

ECode Button::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> txt;
    CStringWrapper::New(String("Button"), (ICharSequence**)&txt);
    return info->SetClassName(txt);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
