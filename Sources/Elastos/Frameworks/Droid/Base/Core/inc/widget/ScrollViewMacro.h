#ifndef __SCROLLVIEWMACRO_H__
#define __SCROLLVIEWMACRO_H__

#define ISCROLLVIEW_METHODS_DECL()                                          \
    CARAPI GetMaxScrollAmount(                                              \
        /* [out] */ Int32* maxAmount);                                      \
                                                                            \
    CARAPI IsFillViewport(                                                  \
        /* [out] */ Boolean* isFilled);                                     \
                                                                            \
    CARAPI SetFillViewport(                                                 \
        /* [in] */ Boolean fillViewport);                                   \
                                                                            \
    CARAPI IsSmoothScrollingEnabled(                                        \
        /* [out] */ Boolean* enabled);                                      \
                                                                            \
    CARAPI SetSmoothScrollingEnabled(                                       \
        /* [in] */ Boolean smoothScrollingEnabled);                         \
                                                                            \
    CARAPI ExecuteKeyEvent(                                                 \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* handled);                                      \
                                                                            \
    CARAPI PageScroll(                                                      \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI FullScroll(                                                      \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI ArrowScroll(                                                     \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI SmoothScrollBy(                                                  \
        /* [in] */ Int32 dx,                                                \
        /* [in] */ Int32 dy);                                               \
                                                                            \
    CARAPI SmoothScrollTo(                                                  \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y);                                                \
                                                                            \
    CARAPI Fling(                                                           \
        /* [in] */ Int32 velocityY);

#define ISCROLLVIEW_METHODS_IMPL(className, superClass)                     \
ECode className::GetMaxScrollAmount(                                        \
    /* [out] */ Int32* maxAmount)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(maxAmount);                                           \
    *maxAmount  = superClass::GetMaxScrollAmount();                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::IsFillViewport(                                            \
    /* [out] */ Boolean* isFilled)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(isFilled);                                            \
    *isFilled  = superClass::IsFillViewport();                              \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetFillViewport(                                           \
    /* [in] */ Boolean fillViewport)                                        \
{                                                                           \
    return superClass::SetFillViewport(fillViewport);                       \
}                                                                           \
                                                                            \
ECode className::IsSmoothScrollingEnabled(                                  \
    /* [out] */ Boolean* enabled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(enabled);                                             \
    *enabled  = superClass::IsSmoothScrollingEnabled();                     \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetSmoothScrollingEnabled(                                 \
    /* [in] */ Boolean smoothScrollingEnabled)                              \
{                                                                           \
    return superClass::SetSmoothScrollingEnabled(smoothScrollingEnabled);   \
}                                                                           \
                                                                            \
ECode className::ExecuteKeyEvent(                                           \
    /* [in] */ IKeyEvent* event,                                            \
    /* [out] */ Boolean* handled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(handled);                                             \
    *handled = superClass::ExecuteKeyEvent(event);                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::PageScroll(                                                \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed = superClass::PageScroll(direction);                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::FullScroll(                                                \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed = superClass::FullScroll(direction);                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::ArrowScroll(                                               \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed = superClass::ArrowScroll(direction);                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SmoothScrollBy(                                            \
    /* [in] */ Int32 dx,                                                    \
    /* [in] */ Int32 dy)                                                    \
{                                                                           \
    return superClass::SmoothScrollBy(dx, dy);                              \
}                                                                           \
                                                                            \
ECode className::SmoothScrollTo(                                            \
    /* [in] */ Int32 x,                                                     \
    /* [in] */ Int32 y)                                                     \
{                                                                           \
    return superClass::SmoothScrollTo(x, y);                                \
}                                                                           \
                                                                            \
ECode className::Fling(                                                     \
    /* [in] */ Int32 velocityY)                                             \
{                                                                           \
    return superClass::Fling(velocityY);                                    \
}


#endif
