
#include "elastos/droid/inputmethodservice/CExtractEditText.h"
#include "elastos/droid/R.h"

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CAR_OBJECT_IMPL(CExtractEditText);
CAR_INTERFACE_IMPL(CExtractEditText, /*EditText*/Object, IExtractEditText);

CExtractEditText::CExtractEditText()
    : mSettingExtractedText(0)
{
}

ECode CExtractEditText::SetExtractedText(
    /* [in] */ IExtractedText* text)
{
    mSettingExtractedText++;
    assert(0 && "TODO");
    // EditText::SetExtractedText(text);

    mSettingExtractedText--;
    return NOERROR;
}

ECode CExtractEditText::PerformClick(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(mIME != NULL);

    assert(0 && "TODO");
    // Boolean tmp = FALSE;
    // if (!(EditText::PerformClick(&tmp), tmp) && mIME != NULL) {
    //     mIME->OnExtractedTextClicked();
    //     *result = TRUE;
    //     return NOERROR;
    // }

    *result = FALSE;
    return NOERROR;
}

ECode CExtractEditText::OnTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean state = FALSE;
    if (mIME != NULL && (mIME->OnExtractTextContextMenuItem(id, &state), state)) {
        // Mode was started on Extracted, needs to be stopped here.
        // Cut and paste will change the text, which stops selection mode.
        if (id == R::id::copy) {
            assert(0 && "TODO");
            // StopSelectionActionMode();
        }

        *result = TRUE;
        return NOERROR;
    }

    assert(0 && "TODO");
    return NOERROR;
    // return EditText::OnTextContextMenuItem(id, result);
}

ECode CExtractEditText::IsInputMethodTarget(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode CExtractEditText::HasWindowFocus(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0 && "TODO");
    return NOERROR;
    // return EditText::IsEnabled(result);
}

ECode CExtractEditText::IsFocused(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0 && "TODO");
    return NOERROR;
    // return EditText::IsEnabled(result);
}

ECode CExtractEditText::HasFocus(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0 && "TODO");
    return NOERROR;
    // return EditText::IsEnabled(result);
}

void CExtractEditText::OnSelectionChanged(
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd)
{
    if (mSettingExtractedText == 0 && mIME != NULL && selStart >= 0 && selEnd >= 0) {
        mIME->OnExtractedSelectionChanged(selStart, selEnd);
    }
}

void CExtractEditText::ViewClicked(
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

void CExtractEditText::DeleteText_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedDeleteText(start, end);
}

void CExtractEditText::ReplaceText_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* text)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedReplaceText(start, end, text);
}

void CExtractEditText::SetSpan_internal(
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedSetSpan(span, start, end, flags);
}

void CExtractEditText::SetCursorPosition_internal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // Do not call the super method.
    // This will change the source TextView instead, which will update the ExtractTextView.
    mIME->OnExtractedSelectionChanged(start, end);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context)
{
    // return EditText::constructor(context, NULL);
    assert(0 && "TODO");
    return NOERROR;
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // return EditText::constructor(context, attrs, 0x0101006e /*com.android.internal.R.attr.editTextStyle*/);
    assert(0 && "TODO");
    return NOERROR;
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode CExtractEditText::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    assert(0 && "TODO");
    return NOERROR;
    // return EditText::constructor(context, attrs, defStyleAttr, defStyleRes);
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
    assert(0 && "TODO");
    // *has = EditText::ComputeVerticalScrollRange() > EditText::ComputeVerticalScrollExtent();
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
