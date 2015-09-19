#include "widget/NumberPickerCustomEditText.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Widget {

NumberPickerCustomEditText::NumberPickerCustomEditText()
{}

NumberPickerCustomEditText::NumberPickerCustomEditText(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ECode NumberPickerCustomEditText::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    EditText::Init(context, attrs);
    return NOERROR;
}


ECode NumberPickerCustomEditText::OnEditorAction(
    /* [in] */ Int32 actionCode)
{
    EditText::OnEditorAction(actionCode);
    if (actionCode == IEditorInfo::IME_ACTION_DONE) {
        ClearFocus();
    }
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos