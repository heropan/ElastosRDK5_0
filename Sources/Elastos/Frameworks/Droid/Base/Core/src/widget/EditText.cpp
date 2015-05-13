
#include "widget/EditText.h"
#include "text/Selection.h"
#include "text/method/CArrowKeyMovementMethod.h"

using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::Method::CArrowKeyMovementMethod;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

const String EditText::EDITTEXT_NAME = String("EditText");

EditText::EditText()
{}

EditText::EditText(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context, attrs, defStyle);
}

Boolean EditText::GetDefaultEditable()
{
    return TRUE;
}

AutoPtr<IMovementMethod> EditText::GetDefaultMovementMethod()
{
    return CArrowKeyMovementMethod::GetInstance();
}

AutoPtr<ICharSequence> EditText::GetText()
{
    return TextView::GetText();
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
    Selection::SetSelection(ISpannable::Probe(GetText().Get()), start, stop);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
 */
ECode EditText::SetSelection(
    /* [in] */ Int32 index)
{
    Selection::SetSelection(ISpannable::Probe(GetText().Get()), index);
    return NOERROR;
}

/**
 * Convenience for {@link Selection#selectAll}.
 */
ECode EditText::SelectAll()
{
    Selection::SelectAll(ISpannable::Probe(GetText().Get()));
    return NOERROR;
}

/**
 * Convenience for {@link Selection#extendSelection}.
 */
ECode EditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    Selection::ExtendSelection(ISpannable::Probe(GetText().Get()), index);
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
    FAIL_RETURN(CStringWrapper::New(EDITTEXT_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode EditText::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(EDITTEXT_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

ECode EditText::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextView::Init(context, attrs, defStyle);
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
