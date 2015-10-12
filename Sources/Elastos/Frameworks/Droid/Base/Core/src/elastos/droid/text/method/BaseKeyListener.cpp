#include <elastos/core/Math.h>

#include "elastos/droid/text/method/BaseKeyListener.h"
#include "elastos/droid/text/TextUtils.h"
#ifdef DROID_CORE
#include "elastos/droid/text/CSelection.h"
#include "elastos/droid/view/CKeyEventHelper.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::Core::CString;
using Elastos::Droid::View::IKeyEventHelper;
using Elastos::Droid::View::CKeyEventHelper;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const AutoPtr<IInterface> BaseKeyListener::OLD_SEL_START = MetaKeyKeyListener::NewNoCopySpan();

Boolean BaseKeyListener::Backspace(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return BackspaceOrForwardDelete(view, content, keyCode, event, FALSE);
}

Boolean BaseKeyListener::ForwardDelete(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return BackspaceOrForwardDelete(view, content, keyCode, event, TRUE);
}

Boolean BaseKeyListener::BackspaceOrForwardDelete(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean isForwardDelete)
{
    // Ensure the key event does not have modifiers except ALT or SHIFT.
    Int32 metaState;
    AutoPtr<IKeyEventHelper> helper;
    CKeyEventHelper::AcquireSingleton((IKeyEventHelper**)&helper);
    Boolean res = FALSE;
    helper->MetaStateHasNoModifiers((event->GetMetaState(&metaState), metaState) & ~(IKeyEvent::META_SHIFT_MASK | IKeyEvent::META_ALT_MASK), &res);
    if (!res) {
        return FALSE;
    }

    // If there is a current selection, delete it.
    if (DeleteSelection(view, content)) {
        return TRUE;
    }

    // Alt+Backspace or Alt+ForwardDelete deletes the current line, if possible.
    Boolean bIsAltPressed;
    if (GetMetaState(content, IKeyEvent::META_ALT_ON, event) == 1) {
        if (DeleteLine(view, content)) {
            return TRUE;
        }
    }

    // Delete a character.
    AutoPtr<ISelection> sel;
    CSelection::AcquireSingleton((ISelection**)&sel);
    /*const*/ Int32 start = 0;
    sel->GetSelectionEnd(content, &start);
    /*const*/ Int32 end;
    Boolean bIsShiftPressed;
    ;
    if (isForwardDelete || (event->IsShiftPressed(&bIsShiftPressed), bIsShiftPressed)
            || GetMetaState(content, IMetaKeyKeyListener::META_SHIFT_ON) == 1) {
        end = TextUtils::GetOffsetAfter(content, start);
    } else {
        end = TextUtils::GetOffsetBefore(content, start);
    }
    if (start != end) {
        content->Delete(Elastos::Core::Math::Min(start, end), Elastos::Core::Math::Max(start, end));
        return TRUE;
    }
    return FALSE;
}

Boolean BaseKeyListener::DeleteSelection(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content)
{
    AutoPtr<ISelection> sel;
    CSelection::AcquireSingleton((ISelection**)&sel);
    Int32 selectionStart = 0,  selectionEnd = 0;
    sel->GetSelectionStart(content, &selectionStart);
    sel->GetSelectionEnd(content, &selectionEnd);
    if (selectionEnd < selectionStart) {
        Int32 temp = selectionEnd;
        selectionEnd = selectionStart;
        selectionStart = temp;
    }
    if (selectionStart != selectionEnd) {
        content->Delete(selectionStart, selectionEnd);
        return TRUE;
    }
    return FALSE;
}

Boolean BaseKeyListener::DeleteLine(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content)
{
    AutoPtr<ITextView> textView = ITextView::Probe(view);
    if(textView!=NULL){
        AutoPtr<ILayout> layout;
        textView->GetLayout((ILayout**)&layout);
        if (layout != NULL) {
            Int32 line, selStart;
            AutoPtr<ISelection> sel;
            CSelection::AcquireSingleton((ISelection**)&sel);
            sel->GetSelectionStart(content, &selStart);
            layout->GetLineForOffset(selStart, &line);
            Int32 start;
            layout->GetLineStart(line, &start);
            Int32 end;
            layout->GetLineEnd(line, &end);
            if (end != start) {
                content->Delete(start, end);
                return TRUE;
            }
        }
    }
    return FALSE;
}

Int32 BaseKeyListener::MakeTextContentType(
    /* [in] */ Capitalize caps,
    /* [in] */ Boolean autoText)
{
    Int32 contentType = IInputType::TYPE_CLASS_TEXT;
    switch (caps) {
        case Capitalize_CHARACTERS:
            contentType |= IInputType::TYPE_TEXT_FLAG_CAP_CHARACTERS;
            break;
        case Capitalize_WORDS:
            contentType |= IInputType::TYPE_TEXT_FLAG_CAP_WORDS;
            break;
        case Capitalize_SENTENCES:
            contentType |= IInputType::TYPE_TEXT_FLAG_CAP_SENTENCES;
            break;
    }
    if (autoText) {
        contentType |= IInputType::TYPE_TEXT_FLAG_AUTO_CORRECT;
    }
    return contentType;
}

Boolean BaseKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean handled;
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DEL:
            handled = Backspace(view, content, keyCode, event);
            break;
        case IKeyEvent::KEYCODE_FORWARD_DEL:
            handled = ForwardDelete(view, content, keyCode, event);
            break;
        default:
            handled = FALSE;
            break;
    }

    if (handled) {
        AdjustMetaAfterKeypress(content);
    }

    return MetaKeyKeyListener::OnKeyDown(view, content, keyCode, event);
}

Boolean BaseKeyListener::OnKeyOther(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ IKeyEvent* event)
{
    Int32 action, keyCode;
    if ((event->GetAction(&action), action) != IKeyEvent::ACTION_MULTIPLE
            || (event->GetKeyCode(&keyCode), keyCode) != IKeyEvent::KEYCODE_UNKNOWN) {
        // Not something we are interested in.
        return FALSE;
    }
    AutoPtr<ISelection> sel;
    CSelection::AcquireSingleton((ISelection**)&sel);
    Int32 selectionStart,  selectionEnd;
    sel->GetSelectionStart(content, &selectionStart);
    sel->GetSelectionEnd(content, &selectionEnd);
    if (selectionEnd < selectionStart) {
        Int32 temp = selectionEnd;
        selectionEnd = selectionStart;
        selectionStart = temp;
    }

    String text;
    event->GetCharacters(&text);
    if (text.IsNullOrEmpty()) {
        return FALSE;
    }

    AutoPtr<ICharSequence> cs;
    CString::New(text, (ICharSequence**)&cs);
    content->Replace(selectionStart, selectionEnd, cs);
    return TRUE;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos