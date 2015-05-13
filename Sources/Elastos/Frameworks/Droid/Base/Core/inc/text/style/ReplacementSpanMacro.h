#ifndef __REPLACEMENTSPANMACRO_H__
#define __REPLACEMENTSPANMACRO_H__

#define IREPLACEMENTSPAN_METHODS_DECL()                                  \
    CARAPI GetSize(                                                      \
        /* [in] */ IPaint* paint,                                       \
        /* [in] */ ICharSequence* text,                                 \
        /* [in] */ Int32 start,                                          \
        /* [in] */ Int32 end,                                            \
        /* [in] */ IPaintFontMetricsInt* fm,                            \
        /* [out] */ Int32* size);                                       \
                                                                         \
    CARAPI Draw(                                                         \
        /* [in] */ ICanvas* canvas,                                     \
        /* [in] */ ICharSequence* text,                                 \
        /* [in] */ Int32 start,                                          \
        /* [in] */ Int32 end,                                            \
        /* [in] */ Float x,                                              \
        /* [in] */ Int32 top,                                            \
        /* [in] */ Int32 y,                                              \
        /* [in] */ Int32 bottom,                                         \
        /* [in] */ IPaint* paint);                                      \


#define IREPLACEMENTSPAN_METHODS_IMPL(className, superClass, overRideClass)  \
ECode className::GetSize(                                                    \
    /* [in] */ IPaint* paint,                                               \
    /* [in] */ ICharSequence* text,                                         \
    /* [in] */ Int32 start,                                                  \
    /* [in] */ Int32 end,                                                    \
    /* [in] */ IPaintFontMetricsInt* fm,                                    \
    /* [out] */ Int32* size)                                                \
{                                                                            \
    VALIDATE_NOT_NULL(size);                                                 \
    *size=superClass::GetSize(paint, text, start, end, fm);                  \
    return NOERROR;                                                          \
}                                                                            \
                                                                             \
ECode className::Draw(                                                       \
    /* [in] */ ICanvas* canvas,                                             \
    /* [in] */ ICharSequence* text,                                         \
    /* [in] */ Int32 start,                                                  \
    /* [in] */ Int32 end,                                                    \
    /* [in] */ Float x,                                                      \
    /* [in] */ Int32 top,                                                    \
    /* [in] */ Int32 y,                                                      \
    /* [in] */ Int32 bottom,                                                 \
    /* [in] */ IPaint* paint)                                               \
{                                                                            \
    superClass::Draw(canvas, text, start, end, x, top, y, bottom, paint);    \
    return NOERROR;                                                          \
}                                                                            \


#endif  //__REPLACEMENTSPANMACRO_H__