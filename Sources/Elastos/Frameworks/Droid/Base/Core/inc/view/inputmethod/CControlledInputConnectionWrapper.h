
#ifndef  __CCONTROLLEDINPUTCONNECTIONWRAPPER_H__
#define  __CCONTROLLEDINPUTCONNECTIONWRAPPER_H__

#include "_CControlledInputConnectionWrapper.h"
#include "view/IInputConnectionWrapper.h"
#include "view/inputmethod/CInputMethodManager.h"

using Elastos::Droid::Internal::View::IInputContextCallback;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CarClass(CControlledInputConnectionWrapper), public IInputConnectionWrapper
{
public:
    CControlledInputConnectionWrapper();

    virtual CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ ILooper* mainLooper,
        /* [in] */ IInputConnection* conn,
        /* [in] */ IInputMethodManager* agmer);

   CARAPI GetTextAfterCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* callback);

    CARAPI GetTextBeforeCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* callback);

    CARAPI GetSelectedText(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* callback);

    CARAPI GetCursorCapsMode(
        /* [in] */ Int32 reqModes,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* callback);

    CARAPI GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* callback);

    CARAPI CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI CommitCompletion(
        /* [in] */ ICompletionInfo* text);

    CARAPI CommitCorrection(
        /* [in] */ ICorrectionInfo* info);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI PerformEditorAction(
        /* [in] */ Int32 id);

    CARAPI PerformContextMenuAction(
        /* [in] */ Int32 id);

    CARAPI SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI FinishComposingText();

    CARAPI SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI ClearMetaKeyStates(
        /* [in] */ Int32 states);

    CARAPI DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength);

    CARAPI BeginBatchEdit();

    CARAPI EndBatchEdit();

    CARAPI ReportFullscreenMode(
        /* [in] */ Boolean enabled);

    CARAPI PerformPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI GetDescription(
        /* [out] */ String* str);

    CARAPI_(Boolean) IsActive();

    CARAPI_(void) Deactivate();

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<CInputMethodManager> mParentInputMethodManager;
    Boolean mActive;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif   //__CCONTROLLEDINPUTCONNECTIONWRAPPER_H__
