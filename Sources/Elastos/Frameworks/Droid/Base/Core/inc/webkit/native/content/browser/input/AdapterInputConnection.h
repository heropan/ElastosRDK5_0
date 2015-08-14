
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_ADAPTERINPUTCONNECTION_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_ADAPTERINPUTCONNECTION_H__

// import android.os.SystemClock;
// import android.text.Editable;
// import android.text.InputType;
// import android.text.Selection;
// import android.text.TextUtils;
// import android.util.Log;
// import android.view.KeyEvent;
// import android.view.View;
// import android.view.inputmethod.BaseInputConnection;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.ExtractedText;
// import android.view.inputmethod.ExtractedTextRequest;

// import com.google.common.annotations.VisibleForTesting;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * InputConnection is created by ContentView.onCreateInputConnection.
 * It then adapts android's IME to chrome's RenderWidgetHostView using the
 * native ImeAdapterAndroid via the class ImeAdapter.
 */
class AdapterInputConnection : public BaseInputConnection
{
public:
    //@VisibleForTesting
    class ImeState
    {
    public:
        ImeState(
            /* [in] */ String text,
            /* [in] */ Int32 selectionStart,
            /* [in] */ Int32 selectionEnd,
            /* [in] */ Int32 compositionStart,
            /* [in] */ Int32 compositionEnd);

    public:
        const String text;
        const Int32 selectionStart;
        const Int32 selectionEnd;
        const Int32 compositionStart;
        const Int32 compositionEnd;
    };

public:
    /**
     * Selection value should be -1 if not known. See EditorInfo.java for details.
     */
    static const Int32 INVALID_SELECTION = -1;
    static const Int32 INVALID_COMPOSITION = -1;

public:
    //@VisibleForTesting
    AdapterInputConnection(
        /* [in] */ IView* view,
        /* [in] */ ImeAdapter* imeAdapter,
        /* [in] */ IEditable* editable,
        /* [in] */ IEditorInfo* outAttrs);

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
    CARAPI_(void) UpdateState(
        /* [in] */ String text,
        /* [in] */ Int32 selectionStart,
        /* [in] */ Int32 selectionEnd,
        /* [in] */ Int32 compositionStart,
        /* [in] */ Int32 compositionEnd,
        /* [in] */ Boolean isNonImeChange);

    /**
     * @return Editable object which contains the state of current focused editable element.
     */
    //@Override
    CARAPI_(AutoPtr<IEditable>) GetEditable();

    /**
     * @see BaseInputConnection#setComposingText(java.lang.CharSequence, int)
     */
    //@Override
    CARAPI_(Boolean) SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    /**
     * @see BaseInputConnection#commitText(java.lang.CharSequence, int)
     */
    //@Override
    CARAPI_(Boolean) CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    /**
     * @see BaseInputConnection#performEditorAction(int)
     */
    //@Override
    CARAPI_(Boolean) PerformEditorAction(
        /* [in] */ Int32 actionCode);

    /**
     * @see BaseInputConnection#performContextMenuAction(int)
     */
    //@Override
    CARAPI_(Boolean) PerformContextMenuAction(
        /* [in] */ Int32 id);

    /**
     * @see BaseInputConnection#getExtractedText(android.view.inputmethod.ExtractedTextRequest,
     *                                           int)
     */
    //@Override
    CARAPI_(AutoPtr<IExtractedText>) GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags);

    /**
     * @see BaseInputConnection#beginBatchEdit()
     */
    //@Override
    CARAPI_(Boolean) BeginBatchEdit();

    /**
     * @see BaseInputConnection#endBatchEdit()
     */
    //@Override
    CARAPI_(Boolean) EndBatchEdit();

    /**
     * @see BaseInputConnection#deleteSurroundingText(int, int)
     */
    //@Override
    CARAPI_(Boolean) DeleteSurroundingText(
        /* [in] */ Int32 beforeLength,
        /* [in] */ Int32 afterLength);

    /**
     * @see BaseInputConnection#sendKeyEvent(android.view.KeyEvent)
     */
    //@Override
    CARAPI_(Boolean) SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * @see BaseInputConnection#finishComposingText()
     */
    //@Override
    CARAPI_(Boolean) FinishComposingText();

    /**
     * @see BaseInputConnection#setSelection(int, int)
     */
    //@Override
    CARAPI_(Boolean) SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Informs the InputMethodManager and InputMethodSession (i.e. the IME) that the text
     * state is no longer what the IME has and that it needs to be updated.
     */
    CARAPI_(void) RestartInput();

    /**
     * @see BaseInputConnection#setComposingRegion(int, int)
     */
    //@Override
    CARAPI_(Boolean) SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Boolean) IsActive();

    //@VisibleForTesting
    CARAPI_(AutoPtr<ImeState>) GetImeStateForTesting();

private:
    /**
     * Sends selection update to the InputMethodManager unless we are currently in a batch edit or
     * if the exact same selection and composition update was sent already.
     */
    CARAPI_(void) UpdateSelectionIfRequired();

    CARAPI_(AutoPtr<InputMethodManagerWrapper>) GetInputMethodManagerWrapper();

    /**
     * This method works around the issue crbug.com/373934 where Blink does not cancel
     * the composition when we send a commit with the empty text.
     *
     * TODO(aurimas) Remove this once crbug.com/373934 is fixed.
     *
     * @param text Text that software keyboard requested to commit.
     * @return Whether the workaround was performed.
     */
    CARAPI_(Boolean) MaybePerformEmptyCompositionWorkaround(
        /* [in] */ ICharSequence* text);

private:
    static const String TAG;
    static const Boolean DEBUG;

    const AutoPtr<IView> mInternalView;
    const AutoPtr<ImeAdapter> mImeAdapter;
    const AutoPtr<IEditable> mEditable;

    Boolean mSingleLine;
    Int32 mNumNestedBatchEdits;

    Int32 mLastUpdateSelectionStart;
    Int32 mLastUpdateSelectionEnd;
    Int32 mLastUpdateCompositionStart;
    Int32 mLastUpdateCompositionEnd;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_ADAPTERINPUTCONNECTION_H__
