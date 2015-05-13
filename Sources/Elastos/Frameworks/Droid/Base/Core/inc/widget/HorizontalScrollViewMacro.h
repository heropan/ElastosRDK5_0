#ifndef __HORIZONTALSCROLLVIEWMACRO_H__
#define __HORIZONTALSCROLLVIEWMACRO_H__

#define IHORIZONTALSCROLLVIEW_METHODS_DECL()                                \
    CARAPI ArrowScroll(                                                     \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI ExecuteKeyEvent(                                                 \
        /* [in] */ IKeyEvent* event,                                        \
        /* [out] */ Boolean* handled);                                      \
                                                                            \
    CARAPI Fling(                                                           \
        /* [in] */ Int32 velocityY);                                        \
                                                                            \
    CARAPI FullScroll(                                                      \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI GetMaxScrollAmount(                                              \
        /* [out] */ Int32* maxScrollAmount);                                \
                                                                            \
    CARAPI IsFillViewport(                                                  \
        /* [out] */ Boolean* isFilled);                                     \
                                                                            \
    CARAPI IsSmoothScrollingEnabled(                                        \
        /* [out] */ Boolean* enabled);                                      \
                                                                            \
    CARAPI PageScroll(                                                      \
        /* [in] */ Int32 direction,                                         \
        /* [out] */ Boolean* consumed);                                     \
                                                                            \
    CARAPI SetFillViewport(                                                 \
        /* [in] */ Boolean fillViewport);                                   \
                                                                            \
    CARAPI SetSmoothScrollingEnabled(                                       \
        /* [in] */ Boolean smoothScrollingEnabled);                         \
                                                                            \
    CARAPI SmoothScrollBy(                                                  \
        /* [in] */ Int32 dx,                                                \
        /* [in] */ Int32 dy);                                               \
                                                                            \
    CARAPI SmoothScrollTo(                                                  \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y);                                                \

#define IHORIZONTALSCROLLVIEW_METHODS_IMPL(className, superClass)           \
ECode className::ArrowScroll(                                               \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed  = superClass::ArrowScroll(direction);                        \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::ExecuteKeyEvent(                                           \
    /* [in] */ IKeyEvent* event,                                            \
    /* [out] */ Boolean* handled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(handled);                                             \
    *handled  = superClass::ExecuteKeyEvent(event);                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::Fling(                                                     \
    /* [in] */ Int32 velocityY)                                             \
{                                                                           \
    return HorizontalScrollView::Fling(velocityY);                          \
}                                                                           \
                                                                            \
ECode className::FullScroll(                                                \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed  = superClass::FullScroll(direction);                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetMaxScrollAmount(                                        \
    /* [out] */ Int32* maxScrollAmount)                                     \
{                                                                           \
    VALIDATE_NOT_NULL(maxScrollAmount);                                     \
    *maxScrollAmount  = superClass::GetMaxScrollAmount();                   \
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
ECode className::IsSmoothScrollingEnabled(                                  \
    /* [out] */ Boolean* enabled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(enabled);                                             \
    *enabled  = superClass::IsSmoothScrollingEnabled();                     \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::PageScroll(                                                \
    /* [in] */ Int32 direction,                                             \
    /* [out] */ Boolean* consumed)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(consumed);                                            \
    *consumed  = superClass::PageScroll(direction);                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetFillViewport(                                           \
    /* [in] */ Boolean fillViewport)                                        \
{                                                                           \
    return HorizontalScrollView::SetFillViewport(fillViewport);             \
}                                                                           \
                                                                            \
ECode className::SetSmoothScrollingEnabled(                                 \
    /* [in] */ Boolean smoothScrollingEnabled)                              \
{                                                                           \
    return superClass::SetSmoothScrollingEnabled(smoothScrollingEnabled);   \
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
}



#endif
