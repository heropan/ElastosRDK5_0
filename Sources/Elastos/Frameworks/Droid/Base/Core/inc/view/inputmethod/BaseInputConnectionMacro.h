#ifndef  __BASEINPUTCONNECTIONMACRO_H__
#define  __BASEINPUTCONNECTIONMACRO_H__

#define BASEINPUTCONNECTION_METHODS_DECL()                                             \
    CARAPI GetEditable(                                                                \
        /* [out] */ IEditable** editable);                                             \
                                                                                       \
    CARAPI BeginBatchEdit(                                                             \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI EndBatchEdit(                                                               \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI ClearMetaKeyStates(                                                         \
        /* [in] */ Int32 states,                                                       \
        /* [out] */ Boolean* clear);                                                   \
                                                                                       \
    CARAPI CommitCompletion(                                                           \
        /* [in] */ ICompletionInfo* text,                                              \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI CommitCorrection(                                                           \
        /* [in] */ ICorrectionInfo* correctionInfo,                                    \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI CommitText(                                                                 \
        /* [in] */ ICharSequence* text,                                                \
        /* [in] */ Int32 newCursorPosition,                                            \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI DeleteSurroundingText(                                                      \
        /* [in] */ Int32 leftLength,                                                   \
        /* [in] */ Int32 rightLength,                                                  \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI FinishComposingText(                                                        \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI GetCursorCapsMode(                                                          \
        /* [in] */ Int32 reqModes,                                                     \
        /* [out] */ Int32* capsMode);                                                  \
                                                                                       \
    CARAPI GetExtractedText(                                                           \
        /* [in] */ IExtractedTextRequest* request,                                     \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ IExtractedText** text);                                            \
                                                                                       \
    CARAPI GetTextBeforeCursor(                                                        \
        /* [in] */ Int32 length,                                                       \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text);                                             \
                                                                                       \
    CARAPI GetSelectedText(                                                            \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text);                                             \
                                                                                       \
    CARAPI GetTextAfterCursor(                                                         \
        /* [in] */ Int32 length,                                                       \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text);                                             \
                                                                                       \
    CARAPI PerformEditorAction(                                                        \
        /* [in] */ Int32 actionCode,                                                   \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI PerformContextMenuAction(                                                   \
        /* [in] */ Int32 id,                                                           \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI PerformPrivateCommand(                                                      \
        /* [in] */ const String& action,                                               \
        /* [in] */ IBundle* data,                                                      \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI SetComposingText(                                                           \
        /* [in] */ ICharSequence* text,                                                \
        /* [in] */ Int32 newCursorPosition,                                            \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI SetComposingRegion(                                                         \
        /* [in] */ Int32 start,                                                        \
        /* [in] */ Int32 end,                                                          \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI SetSelection(                                                               \
        /* [in] */ Int32 start,                                                        \
        /* [in] */ Int32 end,                                                          \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI SendKeyEvent(                                                               \
        /* [in] */ IKeyEvent* event,                                                   \
        /* [out] */ Boolean* state);                                                   \
                                                                                       \
    CARAPI ReportFinish();                                                             \
                                                                                       \
    CARAPI ReportFullscreenMode(                                                       \
        /* [in] */ Boolean enabled,                                                    \
        /* [out] */ Boolean* state);                                                   \



#define BASEINPUTCONNECTION_METHODS_IMPL(className, superClass, overRideClass)         \
    ECode className::GetEditable(                                                      \
        /* [out] */ IEditable** editable)                                              \
    {                                                                                  \
        VALIDATE_NOT_NULL(editable);                                                   \
                                                                                       \
        AutoPtr<IEditable> _editable = superClass::GetEditable();                      \
        *editable = _editable.Get();                                                   \
        if (*editable != NULL) (*editable)->AddRef();                                  \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::BeginBatchEdit(                                                   \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::BeginBatchEdit();                                         \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::EndBatchEdit(                                                     \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::EndBatchEdit();                                           \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::ClearMetaKeyStates(                                               \
        /* [in] */ Int32 states,                                                       \
        /* [out] */ Boolean* clear)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(clear);                                                      \
                                                                                       \
        *clear = superClass::ClearMetaKeyStates(states);                               \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::CommitCompletion(                                                 \
        /* [in] */ ICompletionInfo* text,                                              \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::CommitCompletion(text);                                   \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::CommitCorrection(                                                 \
        /* [in] */ ICorrectionInfo* correctionInfo,                                    \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::CommitCorrection(correctionInfo);                         \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::CommitText(                                                       \
        /* [in] */ ICharSequence* text,                                                \
        /* [in] */ Int32 newCursorPosition,                                            \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::CommitText(text, newCursorPosition);                      \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::DeleteSurroundingText(                                            \
        /* [in] */ Int32 leftLength,                                                   \
        /* [in] */ Int32 rightLength,                                                  \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::DeleteSurroundingText(leftLength, rightLength);           \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::FinishComposingText(                                              \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::FinishComposingText();                                    \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::GetCursorCapsMode(                                                \
        /* [in] */ Int32 reqModes,                                                     \
        /* [out] */ Int32* capsMode)                                                   \
    {                                                                                  \
        VALIDATE_NOT_NULL(capsMode);                                                   \
                                                                                       \
        *capsMode = superClass::GetCursorCapsMode(reqModes);                           \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::GetExtractedText(                                                 \
        /* [in] */ IExtractedTextRequest* request,                                     \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ IExtractedText** text)                                             \
    {                                                                                  \
        VALIDATE_NOT_NULL(text);                                                       \
                                                                                       \
        AutoPtr<IExtractedText> _text = superClass::GetExtractedText(request, flags);  \
        *text = _text.Get();                                                           \
        if (*text != NULL) (*text)->AddRef();                                          \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::GetTextBeforeCursor(                                              \
        /* [in] */ Int32 length,                                                       \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text)                                              \
    {                                                                                  \
        VALIDATE_NOT_NULL(text);                                                       \
                                                                                       \
        AutoPtr<ICharSequence> _text = superClass::GetTextBeforeCursor(length, flags); \
        *text = _text.Get();                                                           \
        if (*text != NULL) (*text)->AddRef();                                          \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::GetSelectedText(                                                  \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text)                                              \
    {                                                                                  \
        VALIDATE_NOT_NULL(text);                                                       \
                                                                                       \
        AutoPtr<ICharSequence> _text = superClass::GetSelectedText(flags);             \
        *text = _text.Get();                                                           \
        if (*text != NULL) (*text)->AddRef();                                          \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::GetTextAfterCursor(                                               \
        /* [in] */ Int32 length,                                                       \
        /* [in] */ Int32 flags,                                                        \
        /* [out] */ ICharSequence** text)                                              \
    {                                                                                  \
        VALIDATE_NOT_NULL(text);                                                       \
                                                                                       \
        AutoPtr<ICharSequence> _text = superClass::GetTextAfterCursor(length, flags);  \
        *text = _text.Get();                                                           \
        if (*text != NULL) (*text)->AddRef();                                          \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::PerformEditorAction(                                              \
        /* [in] */ Int32 actionCode,                                                   \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::PerformEditorAction(actionCode);                          \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::PerformContextMenuAction(                                         \
        /* [in] */ Int32 id,                                                           \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::PerformContextMenuAction(id);                             \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::PerformPrivateCommand(                                            \
        /* [in] */ const String& action,                                               \
        /* [in] */ IBundle* data,                                                      \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::PerformPrivateCommand(action, data);                      \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::SetComposingText(                                                 \
        /* [in] */ ICharSequence* text,                                                \
        /* [in] */ Int32 newCursorPosition,                                            \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::SetComposingText(text, newCursorPosition);                \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::SetComposingRegion(                                               \
        /* [in] */ Int32 start,                                                        \
        /* [in] */ Int32 end,                                                          \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::SetComposingRegion(start, end);                           \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::SetSelection(                                                     \
        /* [in] */ Int32 start,                                                        \
        /* [in] */ Int32 end,                                                          \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::SetSelection(start, end);                                 \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::SendKeyEvent(                                                     \
        /* [in] */ IKeyEvent* event,                                                   \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::SendKeyEvent(event);                                      \
        return NOERROR;                                                                \
    }                                                                                  \
                                                                                       \
    ECode className::ReportFinish()                                                    \
    {                                                                                  \
        return superClass::ReportFinish();                                             \
    }                                                                                  \
                                                                                       \
    ECode className::ReportFullscreenMode(                                             \
        /* [in] */ Boolean enabled,                                                    \
        /* [out] */ Boolean* state)                                                    \
    {                                                                                  \
        VALIDATE_NOT_NULL(state);                                                      \
                                                                                       \
        *state = superClass::ReportFullscreenMode(enabled);                            \
        return NOERROR;                                                                \
    }                                                                                  \


#endif  //__BASEINPUTCONNECTIONMACRO_H__
