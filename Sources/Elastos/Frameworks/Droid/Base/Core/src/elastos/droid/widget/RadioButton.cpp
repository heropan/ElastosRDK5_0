
#include "elastos/droid/widget/RadioButton.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

RadioButton::RadioButton()
{}

RadioButton::RadioButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context, attrs, defStyle)
{}

/**
 * {@inheritDoc}
 * <p>
 * If the radio button is already checked, this method will not toggle the radio button.
 */
ECode RadioButton::Toggle()
{
    // we override to prevent toggle when the radio is already
    // checked (as opposed to check boxes widgets)
    if (!IsChecked()) {
        return CompoundButton::Toggle();
    }

    return NOERROR;
}

ECode RadioButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return CompoundButton::Init(context, attrs, defStyle);
}

ECode RadioButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    CompoundButton::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRadioButton"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    return NOERROR;
}

ECode RadioButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    CompoundButton::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CRadioButton"), (ICharSequence**)&seq);
    info->SetClassName(seq);
    return NOERROR;
}

} // namespace Widget
} // namepsace Droid
} // namespace Elastos
