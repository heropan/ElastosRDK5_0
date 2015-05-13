
#include "inputmethodservice/CExtractEditText.h"

namespace Elastos {
namespace Droid {
namespace InputMethodService {

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Widget::EIID_TextView;

IVIEW_METHODS_IMPL(CExtractEditText, _ExtractEditText);
IDRAWABLECALLBACK_METHODS_IMPL(CExtractEditText, _ExtractEditText);
IKEYEVENTCALLBACK_METHODS_IMPL(CExtractEditText, _ExtractEditText);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CExtractEditText, _ExtractEditText);
ITEXTVIEW_METHODS_IMPL(CExtractEditText, _ExtractEditText);

_ExtractEditText::_ExtractEditText()
    : mSettingExtractedText(0)
{
}

ECode _ExtractEditText::SetExtractedText(
    /* [in] */ IExtractedText* text)
{
    mSettingExtractedText++;
    EditText::SetExtractedText(text);

    mSettingExtractedText--;
    return NOERROR;
}

Boolean _ExtractEditText::PerformClick()
{
    assert(mIME != NULL);

    if (!EditText::PerformClick() && mIME != NULL) {
        mIME->OnExtractedTextClicked();
        return TRUE;
    }

    return TRUE;
}

Boolean _ExtractEditText::OnTextContextMenuItem(
    /* [in] */ Int32 id)
{
    Boolean state = FALSE;
    if (mIME != NULL && (mIME->OnExtractTextContextMenuItem(id, &state), state)) {
        // Mode was started on Extracted, needs to be stopped here.
        // Cut and paste will change the text, which stops selection mode.
        if (id == R::id::copy) StopSelectionActionMode();
        return TRUE;
    }

    return EditText::OnTextContextMenuItem(id);
}

Boolean _ExtractEditText::IsInputMethodTarget()
{
    return TRUE;
}

Boolean _ExtractEditText::HasWindowFocus()
{
    return EditText::IsEnabled();
}

Boolean _ExtractEditText::IsFocused()
{
    return EditText::IsEnabled();
}

Boolean _ExtractEditText::HasFocus()
{
    return EditText::IsEnabled();
}

void _ExtractEditText::OnSelectionChanged(
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd)
{
    if (mSettingExtractedText == 0 && mIME != NULL && selStart >= 0 && selEnd >= 0) {
        mIME->OnExtractedSelectionChanged(selStart, selEnd);
    }
}

void _ExtractEditText::ViewClicked(
    /* [in] */ IInputMethodManager* imm)
{
    // As an instance of this class is supposed to be owned by IMS,
    // and it has a reference to the IMS (the current IME),
    // we just need to call back its onViewClicked() here.
    // It should be good to avoid unnecessary IPCs by doing this as well.
    if (mIME != NULL) {
        mIME->OnViewClicked(FALSE);
    }
}

void _ExtractEditText::DeleteText_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedDeleteText(start, end);
}

void _ExtractEditText::ReplaceText_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* text)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedReplaceText(start, end, text);
}

void _ExtractEditText::SetSpan_internal(
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedSetSpan(span, start, end, flags);
}

void _ExtractEditText::SetCursorPosition_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedSelectionChanged(start, end);
}

CExtractEditText::CExtractEditText()
{}

PInterface CExtractEditText::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }

    return _CExtractEditText::Probe(riid);
}

ECode CExtractEditText::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = EditText::OnPreDraw();
    return NOERROR;
}

ECode CExtractEditText::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return EditText::SetSelection(start, stop);
}


ECode CExtractEditText::SetSelectionEx(
    /* [in] */ Int32 index)
{
    return EditText::SetSelection(index);
}

ECode CExtractEditText::SelectAll()
{
    return EditText::SelectAll();
}

ECode CExtractEditText::ExtendSelection(
    /* [in] */ Int32 index)
{
    return EditText::ExtendSelection(index);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context)
{
    return EditText::Init(context, NULL);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return EditText::Init(context, attrs, 0x0101006e /*com.android.internal.R.attr.editTextStyle*/);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return EditText::Init(context, attrs, defStyle);
}

ECode CExtractEditText::SetIME(
    /* [in] */ IInputMethodService* ime)
{
    mIME = ime;
    return NOERROR;
}

ECode CExtractEditText::StartInternalChanges()
{
    mSettingExtractedText += 1;
    return NOERROR;
}

ECode CExtractEditText::FinishInternalChanges()
{
    mSettingExtractedText -= 1;
    return NOERROR;
}

ECode CExtractEditText::HasVerticalScrollBar(
    /* [out] */ Boolean* has)
{
    assert(has != NULL);
    *has = EditText::ComputeVerticalScrollRange() > EditText::ComputeVerticalScrollExtent();
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
