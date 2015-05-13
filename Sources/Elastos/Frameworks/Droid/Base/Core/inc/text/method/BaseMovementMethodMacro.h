#ifndef __BASEMOVEMENTMETHODMACRO_H__
#define __BASEMOVEMENTMETHODMACRO_H__

#define IBASEMOVEMENTMETHOD_METHODS_DECL()                                  \
    CARAPI Initialize(                                                      \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text);                                     \
                                                                            \
    CARAPI OnKeyDown(                                                       \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                      \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI OnKeyUp(                                                         \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ Int32 keyCode,                                           \
        /* [in] */ IKeyEvent* event,                                      \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI OnKeyOther(                                                      \
        /* [in] */ ITextView* view,                                       \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ IKeyEvent* event,                                      \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI OnTakeFocus(                                                     \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ Int32 direction);                                        \
                                                                            \
    CARAPI OnTrackballEvent(                                                \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ IMotionEvent* event,                                   \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI OnTouchEvent(                                                    \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ IMotionEvent* event,                                   \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI OnGenericMotionEvent(                                            \
        /* [in] */ ITextView* widget,                                     \
        /* [in] */ ISpannable* text,                                      \
        /* [in] */ IMotionEvent* event,                                   \
        /* [out] */ Boolean* result);                                     \
                                                                            \
    CARAPI CanSelectArbitrarily(                                            \
        /* [out] */ Boolean* result);                                     \


#define IBASEMOVEMENTMETHOD_METHODS_IMPL(className, superClass, overRideClass)          \
ECode className::Initialize(                                                            \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text)                                                      \
{                                                                                       \
    superClass::Initialize(widget,text);                                              \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyDown(                                                             \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                      \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnKeyDown(widget, text, keyCode, event);                 \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyUp(                                                               \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ Int32 keyCode,                                                           \
    /* [in] */ IKeyEvent* event,                                                      \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnKeyUp(widget, text, keyCode, event);                   \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnKeyOther(                                                            \
    /* [in] */ ITextView* view,                                                       \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ IKeyEvent* event,                                                      \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnKeyOther(view, text, event);                           \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnTakeFocus(                                                           \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ Int32 direction)                                                         \
{                                                                                       \
    superClass::OnTakeFocus(widget, text, direction);                                 \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnTrackballEvent(                                                      \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ IMotionEvent* event,                                                   \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnTrackballEvent(widget, text, event);                   \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnTouchEvent(                                                          \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ IMotionEvent* event,                                                   \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnTouchEvent(widget, text, event);                       \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::OnGenericMotionEvent(                                                  \
    /* [in] */ ITextView* widget,                                                     \
    /* [in] */ ISpannable* text,                                                      \
    /* [in] */ IMotionEvent* event,                                                   \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::OnGenericMotionEvent(widget, text, event);               \
    return NOERROR;                                                                     \
}                                                                                       \
                                                                                        \
ECode className::CanSelectArbitrarily(                                                  \
    /* [out] */ Boolean* result)                                                      \
{                                                                                       \
    VALIDATE_NOT_NULL(result);                                                         \
    *result =  superClass::CanSelectArbitrarily();                                     \
    return NOERROR;                                                                     \
}                                                                                       \

#endif  //__BASEMOVEMENTMETHODMACRO_H__