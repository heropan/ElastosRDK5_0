#ifndef __BaseKeyListenerMacro_H__
#define __BaseKeyListenerMacro_H__

#define IBaseKeyListener_METHODS_DECL()                                     \
    CARAPI ClearMetaKeyStateEx(                                             \
        /* [in] */ Int64 state,                                             \
        /* [in] */ Int32 which,                                             \
        /* [out] */ Int64* ret);                                            \
                                                                            \
    static CARAPI_(void) ClearMetaKeyStateEx2(                              \
    /* [in] */ IEditable* content,                                          \
    /* [in] */ Int32 states);                                               \
                                                                            \
    CARAPI Backspace(                                                       \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* ret);                                          \
                                                                            \
    CARAPI ForwardDelete(                                                   \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* ret);                                          \


#define IBaseKeyListener_METHODS_IMPL(className, superClass, overRideClass)             \
ECode className::ClearMetaKeyStateEx(                                                   \
    /* [in] */ Int64 state,                                                             \
    /* [in] */ Int32 which,                                                             \
    /* [out] */ Int64* ret)                                                             \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    *ret =  superClass::ClearMetaKeyState(state, which);                                \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
void className::ClearMetaKeyStateEx2(                                                   \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 states)                                                            \
{                                                                                       \
    superClass::ClearMetaKeyState(content, states);                                     \
    return;                                                                             \
}                                                                                       \
                                                                                        \
ECode className::Backspace(                                                             \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                        \
    /* [out] */ Boolean* ret)                                                           \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    *ret =  superClass::Backspace(view, content, keyCode, event);                       \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::ForwardDelete(                                                         \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                        \
    /* [out] */ Boolean* ret)                                                           \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    *ret =  superClass::ForwardDelete(view, content, keyCode, event);                   \
    return NOERROR;                                                                     \
}                                                                                       \


#define IBaseKeyListenerHelper_METHODS_DECL()                               \
    CARAPI ResetMetaState(                                                  \
        /* [in] */ ISpannable* text);                                       \
                                                                            \
    CARAPI GetMetaState(                                                    \
        /* [in] */ ICharSequence* text,                                     \
        /* [out] */ Int32* ret);                                            \
                                                                            \
    CARAPI GetMetaStateEx(                                                  \
        /* [in] */ ICharSequence* text,                                     \
        /* [in] */ Int32 meta,                                              \
        /* [out] */ Int32* ret);                                            \
                                                                            \
    CARAPI AdjustMetaAfterKeypress(                                         \
        /* [in] */ ISpannable* content);                                    \
                                                                            \
    CARAPI IsMetaTracker(                                                   \
        /* [in] */ ICharSequence* text,                                     \
        /* [in] */ IInterface* what,                                        \
        /* [out] */ Boolean* ret);                                          \
                                                                            \
    CARAPI IsSelectingMetaTracker(                                          \
        /* [in] */ ICharSequence* text,                                     \
        /* [in] */ IInterface* what,                                        \
        /* [out] */ Boolean* ret);                                          \
                                                                            \
    CARAPI StartSelecting(                                                  \
        /* [in] */ IView* view,                                             \
        /* [in] */ ISpannable* content);                                    \
                                                                            \
    CARAPI StopSelecting(                                                   \
        /* [in] */ IView* view,                                             \
        /* [in] */ ISpannable* content);                                    \
                                                                            \
    CARAPI ClearMetaKeyState(                                               \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 states);                                           \
                                                                            \
    CARAPI ResetLockedMeta(                                                 \
        /* [in] */ Int64 state,                                             \
        /* [out] */ Int64* ret);                                            \
                                                                            \
    CARAPI GetMetaStateEx2(                                                 \
        /* [in] */ Int64 state,                                             \
        /* [out] */ Int32* ret);                                            \
                                                                            \
    CARAPI GetMetaStateEx3(                                                 \
        /* [in] */ Int64 state,                                             \
        /* [in] */ Int32 meta,                                              \
        /* [out] */ Int32* ret);                                            \
                                                                            \
    CARAPI AdjustMetaAfterKeypressEx(                                       \
        /* [in] */ Int64 state,                                             \
        /* [out] */ Int64* ret);                                            \
                                                                            \
    CARAPI HandleKeyDown(                                                   \
        /* [in] */ Int64 state,                                             \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Int64* ret);                                            \
                                                                            \
    CARAPI HandleKeyUp(                                                     \
        /* [in] */ Int64 state,                                             \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Int64* ret);                                            \


#define IBaseKeyListenerHelper_METHODS_IMPL(className, superClass, overRideClass)             \
ECode className::ResetMetaState(                                                              \
    /* [in] */ ISpannable* text)                                                              \
{                                                                                             \
    superClass::ResetMetaState(text);                                                         \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::GetMetaState(                                                                \
    /* [in] */ ICharSequence* text,                                                           \
    /* [out] */ Int32* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::GetMetaState(text);                                                   \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::GetMetaStateEx(                                                              \
    /* [in] */ ICharSequence* text,                                                           \
    /* [in] */ Int32 meta,                                                                    \
    /* [out] */ Int32* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::GetMetaState(text, meta);                                             \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::AdjustMetaAfterKeypress(                                                     \
    /* [in] */ ISpannable* content)                                                           \
{                                                                                             \
    superClass::AdjustMetaAfterKeypress(content);                                             \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::IsMetaTracker(                                                               \
    /* [in] */ ICharSequence* text,                                                           \
    /* [in] */ IInterface* what,                                                              \
    /* [out] */ Boolean* ret)                                                                 \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::IsMetaTracker(text, what);                                            \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::IsSelectingMetaTracker(                                                      \
    /* [in] */ ICharSequence* text,                                                           \
    /* [in] */ IInterface* what,                                                              \
    /* [out] */ Boolean* ret)                                                                 \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::IsSelectingMetaTracker(text, what);                                   \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::StartSelecting(                                                              \
    /* [in] */ IView* view,                                                                   \
    /* [in] */ ISpannable* content)                                                           \
{                                                                                             \
    superClass::StartSelecting(view, content);                                                \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::StopSelecting(                                                               \
    /* [in] */ IView* view,                                                                   \
    /* [in] */ ISpannable* content)                                                           \
{                                                                                             \
    superClass::StopSelecting(view, content);                                                 \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::ClearMetaKeyState(                                                           \
    /* [in] */ IEditable* content,                                                            \
    /* [in] */ Int32 states)                                                                  \
{                                                                                             \
    superClass::ClearMetaKeyStateEx2(content, states);                                        \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::ResetLockedMeta(                                                             \
    /* [in] */ Int64 state,                                                                   \
    /* [out] */ Int64* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::ResetLockedMeta(state);                                               \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::GetMetaStateEx2(                                                             \
    /* [in] */ Int64 state,                                                                   \
    /* [out] */ Int32* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::GetMetaState(state);                                                  \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::GetMetaStateEx3(                                                             \
    /* [in] */ Int64 state,                                                                   \
    /* [in] */ Int32 meta,                                                                    \
    /* [out] */ Int32* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::GetMetaState(state, meta);                                            \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::AdjustMetaAfterKeypressEx(                                                   \
    /* [in] */ Int64 state,                                                                   \
    /* [out] */ Int64* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::AdjustMetaAfterKeypress(state);                                       \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::HandleKeyDown(                                                               \
    /* [in] */ Int64 state,                                                                   \
    /* [in] */ Int32 keyCode,                                                                 \
    /* [in] */ IKeyEvent* event,                                                              \
    /* [out] */ Int64* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::HandleKeyDown(state, keyCode, event);                                 \
    return NOERROR;                                                                           \
}                                                                                             \
                                                                                              \
ECode className::HandleKeyUp(                                                                 \
    /* [in] */ Int64 state,                                                                   \
    /* [in] */ Int32 keyCode,                                                                 \
    /* [in] */ IKeyEvent* event,                                                              \
    /* [out] */ Int64* ret)                                                                   \
{                                                                                             \
    VALIDATE_NOT_NULL(ret);                                                                   \
    *ret =  superClass::HandleKeyUp(state, keyCode, event);                                   \
    return NOERROR;                                                                           \
}                                                                                             \


#endif  //__BaseKeyListenerMacro_H__