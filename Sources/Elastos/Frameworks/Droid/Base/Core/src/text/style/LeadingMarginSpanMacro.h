#ifndef __LEADINGMARGINSPANMACRO_H__
#define __LEADINGMARGINSPANMACRO_H__

#define ILEADINGMARGINSPAN_METHODS_DECL()                                  \
    CARAPI GetLeadingMargin(                                               \
        /* [in] */ Boolean first,                                          \
        /* [out] */ Int32* offset);                                       \
                                                                           \
    CARAPI DrawLeadingMargin(                                              \
        /* [in] */ ICanvas* c,                                            \
        /* [in] */ IPaint* p,                                             \
        /* [in] */ Int32 x,                                                \
        /* [in] */ Int32 dir,                                              \
        /* [in] */ Int32 top,                                              \
        /* [in] */ Int32 baseline,                                         \
        /* [in] */ Int32 bottom,                                           \
        /* [in] */ ICharSequence* text,                                   \
        /* [in] */ Int32 start,                                            \
        /* [in] */ Int32 end,                                              \
        /* [in] */ Boolean first,                                          \
        /* [in] */ ILayout* layout);                                      \


#define ILEADINGMARGINSPAN_METHODS_IMPL(className, superClass, overRideClass)    \
ECode className::GetLeadingMargin(                                               \
    /* [in] */ Boolean first,                                                    \
    /* [out] */ Int32* offset)                                                  \
{                                                                                \
    VALIDATE_NOT_NULL(offset);                                                   \
    *offset=superClass::GetLeadingMargin(first);                                 \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::DrawLeadingMargin(                                              \
    /* [in] */ ICanvas* c,                                                      \
    /* [in] */ IPaint* p,                                                       \
    /* [in] */ Int32 x,                                                          \
    /* [in] */ Int32 dir,                                                        \
    /* [in] */ Int32 top,                                                        \
    /* [in] */ Int32 baseline,                                                   \
    /* [in] */ Int32 bottom,                                                     \
    /* [in] */ ICharSequence* text,                                             \
    /* [in] */ Int32 start,                                                      \
    /* [in] */ Int32 end,                                                        \
    /* [in] */ Boolean first,                                                    \
    /* [in] */ ILayout* layout)                                                 \
{                                                                               \
    VALIDATE_NOT_NULL(c);                                                       \
    VALIDATE_NOT_NULL(p);                                                       \
    superClass::DrawLeadingMargin(c, p, x, dir, top, baseline, bottom, text, start, end, first, layout); \
    return NOERROR;                                                              \
}                                                                                \


#endif  //__LEADINGMARGINSPANMACRO_H__