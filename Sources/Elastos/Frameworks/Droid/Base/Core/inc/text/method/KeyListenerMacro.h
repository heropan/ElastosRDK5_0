#ifndef __KeyListenerMacro_H__
#define __KeyListenerMacro_H__

#define IKeyListener_METHODS_DECL()                                         \
    CARAPI GetInputType(                                                    \
        /* [out] */ Int32* type);                                           \
                                                                            \
    CARAPI OnKeyDown(                                                       \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* ret);                                          \
                                                                            \
    CARAPI OnKeyUp(                                                         \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* ret);                                          \
                                                                            \
    CARAPI OnKeyOther(                                                      \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* text,                                         \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* result);                                       \
                                                                            \
    CARAPI ClearMetaKeyState(                                               \
        /* [in] */ IView* view,                                             \
        /* [in] */ IEditable* content,                                      \
        /* [in] */ Int32 states);                                           \


#define IKeyListener_METHODS_IMPL(className, superClass, overRideClass)                 \
ECode className::GetInputType(                                                          \
    /* [out] */ Int32* type)                                                            \
{                                                                                       \
    VALIDATE_NOT_NULL(type);                                                            \
    *type =  superClass::GetInputType();                                                \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyDown(                                                             \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                        \
    /* [out] */ Boolean* ret)                                                           \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    *ret =  superClass::OnKeyDown(view, content, keyCode, event);                       \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyUp(                                                               \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                        \
    /* [out] */ Boolean* ret)                                                           \
{                                                                                       \
    VALIDATE_NOT_NULL(ret);                                                             \
    *ret =  superClass::OnKeyUp(view, content, keyCode, event);                         \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyOther(                                                            \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* text,                                                         \
    /* [in] */ IKeyEvent* event,                                                        \
    /* [out] */ Boolean* result)                                                        \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                          \
    *result =  superClass::OnKeyOther(view, text, event);                               \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::ClearMetaKeyState(                                                     \
    /* [in] */ IView* view,                                                             \
    /* [in] */ IEditable* content,                                                      \
    /* [in] */ Int32 states)                                                            \
{                                                                                       \
    superClass::ClearMetaKeyState(view, content, states);                               \
    return NOERROR;                                                                     \
}                                                                                       \

#endif  //__KeyListenerMacro_H__