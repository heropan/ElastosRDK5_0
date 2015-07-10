
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//                AdapterInputConnection::ImeState
//==================================================================

AdapterInputConnection::ImeState::ImeState(
    /* [in] */ String _text,
    /* [in] */ Int32 _selectionStart,
    /* [in] */ Int32 _selectionEnd,
    /* [in] */ Int32 _compositionStart,
    /* [in] */ Int32 _compositionEnd)
    : text(_text)
    , selectionStart(_selectionStart)
    , selectionEnd(_selectionEnd)
    , compositionStart(_compositionStart)
    , compositionEnd(_compositionEnd)
{
}

//==================================================================
//                      AdapterInputConnection
//==================================================================

/**
 * InputConnection is created by ContentView.onCreateInputConnection.
 * It then adapts android's IME to chrome's RenderWidgetHostView using the
 * native ImeAdapterAndroid via the class ImeAdapter.
 */
public class AdapterInputConnection extends BaseInputConnection {
private static final String TAG = "AdapterInputConnection";
private static final boolean DEBUG = false;
/**
 * Selection value should be -1 if not known. See EditorInfo.java for details.
 */
public static final int INVALID_SELECTION = -1;
public static final int INVALID_COMPOSITION = -1;

//@VisibleForTesting
AdapterInputConnection::AdapterInputConnection(
    /* [in] */ IView* view,
    /* [in] */ ImeAdapter* imeAdapter,
    /* [in] */ IEditable* editable,
    /* [in] */ IEditorInfo* outAttrs)
    : BaseInputConnection(view, TRUE)
    , mNumNestedBatchEdits(0)
    , mLastUpdateSelectionStart(INVALID_SELECTION)
    , mLastUpdateSelectionEnd(INVALID_SELECTION)
    , mLastUpdateCompositionStart(INVALID_COMPOSITION)
    , mLastUpdateCompositionEnd(INVALID_COMPOSITION)
    , mInternalView(view)
    , mImeAdapter(imeAdapter)
{
    mImeAdapter->SetInputConnection(this);
    mEditable = editable;
    // The editable passed in might have been in use by a prior keyboard and could have had
    // prior composition spans set.  To avoid keyboard conflicts, remove all composing spans
    // when taking ownership of an existing Editable.
    RemoveComposingSpans(mEditable);
    mSingleLine = TRUE;
    outAttrs->SetImeOptions(IEditorInfo::IME_FLAG_NO_FULLSCREEN
            | IEditorInfo::IME_FLAG_NO_EXTRACT_UI);
    outAttrs->SetInputType(IEditorInfo::TYPE_CLASS_TEXT
            | IEditorInfo::TYPE_TEXT_VARIATION_WEB_EDIT_TEXT);

    if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeText) {
        // Normal text field
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType |= IEditorInfo::TYPE_TEXT_FLAG_AUTO_CORRECT;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_GO;
        outAttrs->SetImeOptions(imeOptions);
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeTextArea ||
            imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeContentEditable) {
        // TextArea or contenteditable.
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType |= IEditorInfo::TYPE_TEXT_FLAG_MULTI_LINE
                | IEditorInfo::TYPE_TEXT_FLAG_CAP_SENTENCES
                | IEditorInfo::TYPE_TEXT_FLAG_AUTO_CORRECT;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_NONE;
        outAttrs->SetInputType(imeOptions);
        mSingleLine = FALSE;
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypePassword) {
        // Password
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType = IInputType::TYPE_CLASS_TEXT
                | IInputType::TYPE_TEXT_VARIATION_WEB_PASSWORD;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_GO;
        outAttrs->SetImeOptions(imeOptions);
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeSearch) {
        // Search
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_SEARCH;
        outAttrs->SetImeOptions(imeOptions);
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeUrl) {
        // Url
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType = IInputType::TYPE_CLASS_TEXT
                | IInputType::TYPE_TEXT_VARIATION_URI;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_GO;
        outAttrs->SetImeOptions(imeOptions);
        return imeOptions;
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeEmail) {
        // Email
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType = IInputType::TYPE_CLASS_TEXT
                | IInputType::TYPE_TEXT_VARIATION_WEB_EMAIL_ADDRESS;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_GO;
        outAttrs->SetImeOptions(imeOptions);
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeTel) {
        // Telephone
        // Number and telephone do not have both a Tab key and an
        // action in default OSK, so set the action to NEXT
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType = IInputType::TYPE_CLASS_PHONE;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_NEXT;
        outAttrs->SetImeOptions(imeOptions);
    }
    else if (imeAdapter->GetTextInputType() == ImeAdapter::sTextInputTypeNumber) {
        // Number
        Int32 inputType;
        outAttrs->GetInputType(&inputType);
        inputType = IInputType::TYPE_CLASS_NUMBER
                | IInputType::TYPE_NUMBER_VARIATION_NORMAL
                | IInputType::TYPE_NUMBER_FLAG_DECIMAL;
        outAttrs->SetInputType(inputType);
        Int32 imeOptions;
        outAttrs->GetImeOptions(&imeOptions);
        imeOptions |= IEditorInfo::IME_ACTION_NEXT;
        outAttrs->SetImeOptions(imeOptions);
    }

    AutoPtr<ISelection> selection;
    CSelection::AcquireSingleton((ISelection**)&selection);
    Int32 start, end;
    selection->GetSelectionStart(mEditable, &start);
    selection->GetSelectionEnd(mEditable, &end);
    outAttrs->SetInitialSelStart(start);
    outAttrs->SetInitialSelEnd(end);
    mLastUpdateSelectionStart = start;
    mLastUpdateSelectionEnd = end;

    Int32 initialSelStart, initialSelEnd;
    selection->GetinitialSelStart(&initialSelStart);
    selection->GetinitialSelEnd(&initialSelEnd)
    selection->SetSelection(mEditable, initialSelStart, initialSelEnd);
    UpdateSelectionIfRequired();
}

/**
 * Updates the AdapterInputConnection's internal representation of the text being edited and
 * its selection and composition properties. The resulting Editable is accessible through the
 * getEditable() method. If the text has not changed, this also calls updateSelection on the
 * InputMethodManager.
 *
 * @param text The String contents of the field being edited.
 * @param selectionStart The character offset of the selection start, or the caret position if
 *                       there is no selection.
 * @param selectionEnd The character offset of the selection end, or the caret position if there
 *                     is no selection.
 * @param compositionStart The character offset of the composition start, or -1 if there is no
 *                         composition.
 * @param compositionEnd The character offset of the composition end, or -1 if there is no
 *                       selection.
 * @param isNonImeChange True when the update was caused by non-IME (e.g. Javascript).
 */
//@VisibleForTesting
void AdapterInputConnection::UpdateState(
    /* [in] */ String text,
    /* [in] */ Int32 selectionStart,
    /* [in] */ Int32 selectionEnd,
    /* [in] */ Int32 compositionStart,
    /* [in] */ Int32 compositionEnd,
    /* [in] */ Boolean isNonImeChange)
{
    if (DEBUG) {
        // Log.w(TAG, "updateState [" + text + "] [" + selectionStart + " " + selectionEnd + "] ["
        //         + compositionStart + " " + compositionEnd + "] [" + isNonImeChange + "]");
    }
    // If this update is from the IME, no further state modification is necessary because the
    // state should have been updated already by the IM framework directly.
    if (!isNonImeChange) return;

    // Non-breaking spaces can cause the IME to get confused. Replace with normal spaces.
    text = text.Replace('\u00A0', ' ');

    selectionStart = Math::Min(selectionStart, text.GetLength());
    selectionEnd = Math::Min(selectionEnd, text.GetLength());
    compositionStart = Math::Min(compositionStart, text.GetLength());
    compositionEnd = Math::Min(compositionEnd, text.GetLength());

    String prevText;
    mEditable->ToString(&prevText);
    Boolean textUnchanged = prevText.Equals(text);

    if (!textUnchanged) {
        mEditable->Replace(0, mEditable.GetLength(), text);
    }

    AutoPtr<ISelection> selection;
    CSelection::AcquireSingleton((ISelection**)&selection);
    selection->SetSelection(mEditable, selectionStart, selectionEnd);

    if (compositionStart == compositionEnd) {
        RemoveComposingSpans(mEditable);
    }
    else {
        BaseInputConnection::SetComposingRegion(compositionStart, compositionEnd);
    }

    UpdateSelectionIfRequired();
}

/**
 * @return Editable object which contains the state of current focused editable element.
 */
//@Override
AutoPtr<IEditable> AdapterInputConnection::GetEditable()
{
    return mEditable;
}

/**
 * Sends selection update to the InputMethodManager unless we are currently in a batch edit or
 * if the exact same selection and composition update was sent already.
 */
void AdapterInputConnection::UpdateSelectionIfRequired()
{
    if (mNumNestedBatchEdits != 0) return;

    AutoPtr<ISelection> selection;
    CSelection::AcquireSingleton((ISelection**)&selection);
    Int32 selectionStart;
    selection->GetSelectionStart(mEditable, &selectionStart);
    Int32 selectionEnd;
    selection->GetSelectionEnd(mEditable, &selectionEnd);
    Int32 compositionStart = GetComposingSpanStart(mEditable);
    Int32 compositionEnd = GetComposingSpanEnd(mEditable);
    // Avoid sending update if we sent an exact update already previously.
    if (mLastUpdateSelectionStart == selectionStart &&
            mLastUpdateSelectionEnd == selectionEnd &&
            mLastUpdateCompositionStart == compositionStart &&
            mLastUpdateCompositionEnd == compositionEnd) {
        return;
    }

    if (DEBUG) {
        // Log.w(TAG, "updateSelectionIfRequired [" + selectionStart + " " + selectionEnd + "] ["
        //         + compositionStart + " " + compositionEnd + "]");
    }

    // updateSelection should be called every time the selection or composition changes
    // if it happens not within a batch edit, or at the end of each top level batch edit.
    GetInputMethodManagerWrapper()->UpdateSelection(mInternalView,
            selectionStart, selectionEnd, compositionStart, compositionEnd);
    mLastUpdateSelectionStart = selectionStart;
    mLastUpdateSelectionEnd = selectionEnd;
    mLastUpdateCompositionStart = compositionStart;
    mLastUpdateCompositionEnd = compositionEnd;
}

/**
 * @see BaseInputConnection#setComposingText(java.lang.CharSequence, int)
 */
//@Override
Boolean AdapterInputConnection::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
//    if (DEBUG) Log.w(TAG, "setComposingText [" + text + "] [" + newCursorPosition + "]");
    if (MaybePerformEmptyCompositionWorkaround(text)) return TRUE;
    BaseInputConnection::SetComposingText(text, newCursorPosition);
    UpdateSelectionIfRequired();
    return mImeAdapter->CheckCompositionQueueAndCallNative(text, newCursorPosition, false);
}

/**
 * @see BaseInputConnection#commitText(java.lang.CharSequence, int)
 */
//@Override
Boolean AdapterInputConnection::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
//    if (DEBUG) Log.w(TAG, "commitText [" + text + "] [" + newCursorPosition + "]");
    if (MaybePerformEmptyCompositionWorkaround(text)) return TRUE;
    BaseInputConnection::CommitText(text, newCursorPosition);
    UpdateSelectionIfRequired();
    return mImeAdapter->CheckCompositionQueueAndCallNative(text, newCursorPosition,
            text.GetLength() > 0);
}

/**
 * @see BaseInputConnection#performEditorAction(int)
 */
//@Override
Boolean AdapterInputConnection::PerformEditorAction(
    /* [in] */ Int32 actionCode)
{
//    if (DEBUG) Log.w(TAG, "performEditorAction [" + actionCode + "]");
    if (actionCode == IEditorInfo::IME_ACTION_NEXT) {
        RestartInput();
        // Send TAB key event
        Int64 timeStampMs = SystemClock::UptimeMillis();
        mImeAdapter->SendSyntheticKeyEvent(
                ImeAdapter::sEventTypeRawKeyDown, timeStampMs, IKeyEvent::KEYCODE_TAB, 0);
    }
    else {
        mImeAdapter->SendKeyEventWithKeyCode(IKeyEvent::KEYCODE_ENTER,
                IKeyEvent::FLAG_SOFT_KEYBOARD | IKeyEvent::FLAG_KEEP_TOUCH_MODE
                | IKeyEvent::FLAG_EDITOR_ACTION);
    }

    return TRUE;
}

/**
 * @see BaseInputConnection#performContextMenuAction(int)
 */
//@Override
Boolean AdapterInputConnection::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
//    if (DEBUG) Log.w(TAG, "performContextMenuAction [" + id + "]");
    switch (id) {
        case android::R::id::selectAll:
            return mImeAdapter->SelectAll();
        case android::R::id::cut:
            return mImeAdapter->Cut();
        case android::R::id::copy:
            return mImeAdapter->Copy();
        case android::R::id::paste:
            return mImeAdapter->Paste();
        default:
            return FALSE;
    }
}

/**
 * @see BaseInputConnection#getExtractedText(android.view.inputmethod.ExtractedTextRequest,
 *                                           int)
 */
//@Override
AutoPtr<IExtractedText> AdapterInputConnection::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags)
{
//    if (DEBUG) Log.w(TAG, "getExtractedText");
    AutoPtr<IExtractedText> et;
    CExtractedText::New((IExtractedText**)&et);
    String str;
    mEditable->ToString(&str);
    et->SetText(str);
    Int32 length;
    mEditable->GetLength(&length);
    et->SetPartialEndOffset(length);
    AutoPtr<ISelection> selection;
    CSelection::AcquireSingleton((ISelection**)&selection);
    Int32 start;
    selection->GetSelectionStart(mEditable, &start);
    et->SetSelectionStart(start)
    Int32 end;
    selection->GetSelectionEnd(mEditable, &end);
    et->SetSelectionEnd(end);
    et->SetFlags(mSingleLine ? IExtractedText::FLAG_SINGLE_LINE : 0);

    return et;
}

/**
 * @see BaseInputConnection#beginBatchEdit()
 */
//@Override
Boolean AdapterInputConnection::BeginBatchEdit()
{
//    if (DEBUG) Log.w(TAG, "beginBatchEdit [" + (mNumNestedBatchEdits == 0) + "]");
    mNumNestedBatchEdits++;
    return TRUE;
}

/**
 * @see BaseInputConnection#endBatchEdit()
 */
//@Override
Boolean AdapterInputConnection::EndBatchEdit()
{
    if (mNumNestedBatchEdits == 0) return FALSE;
    --mNumNestedBatchEdits;

//    if (DEBUG) Log.w(TAG, "endBatchEdit [" + (mNumNestedBatchEdits == 0) + "]");

    if (mNumNestedBatchEdits == 0) UpdateSelectionIfRequired();

    return (mNumNestedBatchEdits != 0);
}

/**
 * @see BaseInputConnection#deleteSurroundingText(int, int)
 */
//@Override
Boolean AdapterInputConnection::DeleteSurroundingText(
    /* [in] */ Int32 beforeLength,
    /* [in] */ Int32 afterLength)
{
    if (DEBUG) {
//        Log.w(TAG, "deleteSurroundingText [" + beforeLength + " " + afterLength + "]");
    }

    AutoPtr<ISelection> selection;
    CSelection::AcquireSingleton((ISelection**)&selection);
    Int32 availableBefore;
    selection->GetSelectionStart(mEditable, &availableBefore);
    Int32 end;
    selection->GetSelectionEnd(mEditable, &end);
    Int32 length;
    mEditable->GetLength(&length);
    Int32 availableAfter = length - end;
    beforeLength = Math::Min(beforeLength, availableBefore);
    afterLength = Math::Min(afterLength, availableAfter);
    BaseInputConnection::DeleteSurroundingText(beforeLength, afterLength);
    UpdateSelectionIfRequired();
    return mImeAdapter->DeleteSurroundingText(beforeLength, afterLength);
}

/**
 * @see BaseInputConnection#sendKeyEvent(android.view.KeyEvent)
 */
//@Override
Boolean AdapterInputConnection::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (DEBUG) {
//        Log.w(TAG, "sendKeyEvent [" + event.getAction() + "] [" + event.getKeyCode() + "]");
    }
    // If this is a key-up, and backspace/del or if the key has a character representation,
    // need to update the underlying Editable (i.e. the local representation of the text
    // being edited).
    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_UP) {
        Int32 code;
        event->GetKeyCode(&code);
        if (code == IKeyEvent::KEYCODE_DEL) {
            DeleteSurroundingText(1, 0);
            return TRUE;
        }
        else if (code == IKeyEvent::KEYCODE_FORWARD_DEL) {
            DeleteSurroundingText(0, 1);
            return TRUE;
        }
        else {
            Int32 unicodeChar;
            event->GetUnicodeChar(&unicodeChar);
            if (unicodeChar != 0) {
                AutoPtr<ISelection> selection;
                CSelection::AcquireSingleton((ISelection**)&selection);
                Int32 selectionStart;
                selection->GetSelectionStart(mEditable, &selectionStart);
                Int32 selectionEnd;
                selection->GetSelectionEnd(mEditable, &selectionEnd);
                if (selectionStart > selectionEnd) {
                    Int32 temp = selectionStart;
                    selectionStart = selectionEnd;
                    selectionEnd = temp;
                }

                String unicodeCharStr;
                unicodeChar->ToString(&unicodeCharStr);
                mEditable->Replace(selectionStart, selectionEnd,
                        unicodeCharStr);
            }
        }
    }
    else if (action == IKeyEvent::ACTION_DOWN) {
        // TODO(aurimas): remove this workaround when crbug.com/278584 is fixed.
        Int32 code;
        event->GetKeyCode(&code);
        if (code == IKeyEvent::KEYCODE_ENTER) {
            BeginBatchEdit();
            FinishComposingText();
            mImeAdapter->TranslateAndSendNativeEvents(event);
            EndBatchEdit();
            return TRUE;
        }
        else if (code == IKeyEvent::KEYCODE_DEL) {
            return TRUE;
        }
        else if (code == IKeyEvent::KEYCODE_FORWARD_DEL) {
            return TRUE;
        }
    }

    mImeAdapter->TranslateAndSendNativeEvents(event);

    return TRUE;
}

/**
 * @see BaseInputConnection#finishComposingText()
 */
//@Override
Boolean AdapterInputConnection::FinishComposingText()
{
//    if (DEBUG) Log.w(TAG, "finishComposingText");
    if (GetComposingSpanStart(mEditable) == GetComposingSpanEnd(mEditable)) {
        return TRUE;
    }

    BaseInputConnection::FinishComposingText();
    UpdateSelectionIfRequired();
    mImeAdapter->FinishComposingText();

    return TRUE;
}

/**
 * @see BaseInputConnection#setSelection(int, int)
 */
//@Override
Boolean AdapterInputConnection::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
//    if (DEBUG) Log.w(TAG, "setSelection [" + start + " " + end + "]");
    Int32 textLength;
    mEditable->GetLength(&textLength);
    if (start < 0 || end < 0 || start > textLength || end > textLength) return TRUE;
    BaseInputConnection::SetSelection(start, end);
    UpdateSelectionIfRequired();
    return mImeAdapter->SetEditableSelectionOffsets(start, end);
}

/**
 * Informs the InputMethodManager and InputMethodSession (i.e. the IME) that the text
 * state is no longer what the IME has and that it needs to be updated.
 */
void AdapterInputConnection::RestartInput()
{
//    if (DEBUG) Log.w(TAG, "restartInput");
    GetInputMethodManagerWrapper()->RestartInput(mInternalView);
    mNumNestedBatchEdits = 0;
}

/**
 * @see BaseInputConnection#setComposingRegion(int, int)
 */
//@Override
Boolean AdapterInputConnection::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
//    if (DEBUG) Log.w(TAG, "setComposingRegion [" + start + " " + end + "]");
    Int32 textLength;
    mEditable->GetLength(&textLength);
    Int32 a = Math::Min(start, end);
    Int32 b = Math::Max(start, end);
    if (a < 0) a = 0;
    if (b < 0) b = 0;
    if (a > textLength) a = textLength;
    if (b > textLength) b = textLength;

    if (a == b) {
        RemoveComposingSpans(mEditable);
    }
    else {
        BaseInputConnection::SetComposingRegion(a, b);
    }

    UpdateSelectionIfRequired();

    return mImeAdapter->SetComposingRegion(a, b);
}

Boolean AdapterInputConnection::IsActive() {
    return GetInputMethodManagerWrapper()->IsActive(mInternalView);
}

InputMethodManagerWrapper AdapterInputConnection::GetInputMethodManagerWrapper()
{
    return mImeAdapter->GetInputMethodManagerWrapper();
}

/**
 * This method works around the issue crbug.com/373934 where Blink does not cancel
 * the composition when we send a commit with the empty text.
 *
 * TODO(aurimas) Remove this once crbug.com/373934 is fixed.
 *
 * @param text Text that software keyboard requested to commit.
 * @return Whether the workaround was performed.
 */
Boolean AdapterInputConnection::MaybePerformEmptyCompositionWorkaround(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<ISelection> sel;
    CSelection::AcquireSingleton((ISelection**)&sel);
    Int32 selectionStart;
    sel->GetSelectionStart(mEditable, &selectionStart);
    Int32 selectionEnd;
    sel->GetSelectionEnd(mEditable, &selectionEnd);
    Int32 compositionStart = GetComposingSpanStart(mEditable);
    Int32 compositionEnd = GetComposingSpanEnd(mEditable);
    if (TextUtils::IsEmpty(text) && (selectionStart == selectionEnd)
            && compositionStart != INVALID_COMPOSITION
            && compositionEnd != INVALID_COMPOSITION) {
        BeginBatchEdit();
        FinishComposingText();
        Int32 selection;
        sel->GetSelectionStart(mEditable);
        DeleteSurroundingText(selection - compositionStart, selection - compositionEnd);
        EndBatchEdit();
        return TRUE;
    }
    return false;
}

//@VisibleForTesting
AutoPtr<ImeState> AdapterInputConnection::GetImeStateForTesting()
{
    String text;
    mEditable->ToString(&text);
    AutoPtr<ISelection> selection;
    Int32 selectionStart;
    selection->GetSelectionStart(mEditable, &selectionStart);
    Int32 selectionEnd;
    selection->GetSelectionEnd(mEditable, &selectionEnd);
    Int32 compositionStart = GetComposingSpanStart(mEditable);
    Int32 compositionEnd = GetComposingSpanEnd(mEditable);
    AutoPtr<ImeState> imeState = new ImeState(text, selectionStart, selectionEnd, compositionStart, compositionEnd);
    return imeState;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
