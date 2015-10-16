
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/view/inputmethod/CControlledInputConnectionWrapper.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CControlledInputConnectionWrapper::CControlledInputConnectionWrapper()
    : mActive(FALSE)
{
}

PInterface CControlledInputConnectionWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInputConnectionWrapper) {
        return reinterpret_cast<PInterface>((IInputConnectionWrapper*)this);
    }
    return _CControlledInputConnectionWrapper::Probe(riid);
}

ECode CControlledInputConnectionWrapper::constructor(
    /* [in] */ ILooper* mainLooper,
    /* [in] */ IInputConnection* conn,
    /* [in] */ IInputMethodManager* agmer)
{
    IInputConnectionWrapper::Init(mainLooper, conn);
    mParentInputMethodManager = (CInputMethodManager*)agmer;
    mActive = TRUE;
    return NOERROR;
}

ECode CControlledInputConnectionWrapper::GetTextAfterCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetTextAfterCursor(length, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetTextBeforeCursor(
    /* [in] */ Int32 length,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetTextBeforeCursor(length, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetSelectedText(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetSelectedText(flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetCursorCapsMode(
    /* [in] */ Int32 reqModes,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetCursorCapsMode(reqModes, seq, callback);
}

ECode CControlledInputConnectionWrapper::GetExtractedText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 seq,
    /* [in] */ IInputContextCallback* callback)
{
    return IInputConnectionWrapper::GetExtractedText(request, flags, seq, callback);
}

ECode CControlledInputConnectionWrapper::CommitText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    return IInputConnectionWrapper::CommitText(text, newCursorPosition);
}

ECode CControlledInputConnectionWrapper::CommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    return IInputConnectionWrapper::CommitCompletion(text);
}

ECode CControlledInputConnectionWrapper::CommitCorrection(
    /* [in] */ ICorrectionInfo* info)
{
    return IInputConnectionWrapper::CommitCorrection(info);
}

ECode CControlledInputConnectionWrapper::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return IInputConnectionWrapper::SetSelection(start, end);
}

ECode CControlledInputConnectionWrapper::PerformEditorAction(
    /* [in] */ Int32 id)
{
    return IInputConnectionWrapper::PerformEditorAction(id);
}

ECode CControlledInputConnectionWrapper::PerformContextMenuAction(
    /* [in] */ Int32 id)
{
    return IInputConnectionWrapper::PerformContextMenuAction(id);
}

ECode CControlledInputConnectionWrapper::SetComposingRegion(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return IInputConnectionWrapper::SetComposingRegion(start, end);
}

ECode CControlledInputConnectionWrapper::SetComposingText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 newCursorPosition)
{
    return IInputConnectionWrapper::SetComposingText(text, newCursorPosition);
}

ECode CControlledInputConnectionWrapper::FinishComposingText()
{
    return IInputConnectionWrapper::FinishComposingText();
}

ECode CControlledInputConnectionWrapper::SendKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return IInputConnectionWrapper::SendKeyEvent(event);
}

ECode CControlledInputConnectionWrapper::ClearMetaKeyStates(
    /* [in] */ Int32 states)
{
    return IInputConnectionWrapper::ClearMetaKeyStates(states);
}

ECode CControlledInputConnectionWrapper::DeleteSurroundingText(
    /* [in] */ Int32 leftLength,
    /* [in] */ Int32 rightLength)
{
    return IInputConnectionWrapper::DeleteSurroundingText(leftLength, rightLength);
}

ECode CControlledInputConnectionWrapper::BeginBatchEdit()
{
    return IInputConnectionWrapper::BeginBatchEdit();
}

ECode CControlledInputConnectionWrapper::EndBatchEdit()
{
    return IInputConnectionWrapper::EndBatchEdit();
}

ECode CControlledInputConnectionWrapper::ReportFullscreenMode(
    /* [in] */ Boolean enabled)
{
    return IInputConnectionWrapper::ReportFullscreenMode(enabled);
}

ECode CControlledInputConnectionWrapper::PerformPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return IInputConnectionWrapper::PerformPrivateCommand(action, data);
}

ECode CControlledInputConnectionWrapper::GetDescription(
    /* [out] */ String* str)
{
    return IInputConnectionWrapper::GetDescription(str);
}

Boolean CControlledInputConnectionWrapper::IsActive()
{
    return mParentInputMethodManager->mActive && mActive;
}

void CControlledInputConnectionWrapper::Deactivate()
{
    mActive = FALSE;
}

ECode CControlledInputConnectionWrapper::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
