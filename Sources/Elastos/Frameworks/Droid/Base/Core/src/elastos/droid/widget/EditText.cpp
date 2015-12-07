
#include "elastos/droid/widget/EditText.h"
#include "elastos/droid/text/Selection.h"
#include "elastos/droid/text/method/CArrowKeyMovementMethod.h"

using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::Method::CArrowKeyMovementMethod;
using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Widget {

const String EditText::EDITTEXT_NAME = String("EditText");

CAR_INTERFACE_IMPL(EditText, TextView, IEditText)

EditText::EditText()
{}


ECode EditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 defStyleRes)
{
    return TextView::constructor(context, attrs, defStyle, defStyleRes);
}

Boolean EditText::GetDefaultEditable()
{
    return TRUE;
}

AutoPtr<IMovementMethod> EditText::GetDefaultMovementMethod()
{
    AutoPtr<IMovementMethod> result;
    CArrowKeyMovementMethod::GetInstance((IMovementMethod**)&result);
    return result;
}

ECode EditText::GetText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    return TextView::GetText(text);
}

ECode EditText::SetText(
    /* [in] */ ICharSequence* text,
    /* [in] */ BufferType type)
{
    return TextView::SetText(text, BufferType_EDITABLE);
}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32, Int32)}.
 */
ECode EditText::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    AutoPtr<ICharSequence> seq;
    GetText((ICharSequence**)&seq);
    Selection::SetSelection(ISpannable::Probe(seq), start, stop);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
 */
ECode EditText::SetSelection(
    /* [in] */ Int32 index)
{
    AutoPtr<ICharSequence> seq;
    GetText((ICharSequence**)&seq);
    Selection::SetSelection(ISpannable::Probe(seq), index);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#selectAll}.
 */
ECode EditText::SelectAll()
{
    AutoPtr<ICharSequence> seq;
    GetText((ICharSequence**)&seq);
    Selection::SelectAll(ISpannable::Probe(seq));
    return NOERROR;
}

/**
 * Convenience for {@link Selection#extendSelection}.
 */
ECode EditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    AutoPtr<ICharSequence> seq;
    GetText((ICharSequence**)&seq);
    Selection::ExtendSelection(ISpannable::Probe(seq), index);
    return NOERROR;
}

ECode EditText::SetEllipsize(
    /* [in] */ TextUtilsTruncateAt ellipsis)
{
    if (ellipsis == Elastos::Droid::Text::TextUtilsTruncateAt_MARQUEE) {
//        throw new IllegalArgumentException("EditText cannot use the ellipsize mode "
//                + "TextUtils.TruncateAt.MARQUEE");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return TextView::SetEllipsize(ellipsis);
}

ECode EditText::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CString::New(EDITTEXT_NAME, (ICharSequence**)&seq));
    IAccessibilityRecord::Probe(event)->SetClassName(seq);
    return NOERROR;
}

ECode EditText::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CString::New(EDITTEXT_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

ECode EditText::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* res)
{
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_SET_TEXT:
        {
            AutoPtr<ICharSequence> text;
            if (arguments != NULL) {
                arguments->GetCharSequence(IAccessibilityNodeInfo::ACTION_ARGUMENT_SET_TEXT_CHARSEQUENCE,
                    (ICharSequence**)&text);
            }
            SetText(text);
            Int32 len;
            if (text != NULL && (text->GetLength(&len), len) > 0) {
                SetSelection(len);
            }
            return TRUE;
        }
        default: {
            return TextView::PerformAccessibilityAction(action, arguments);
        }
    }
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
