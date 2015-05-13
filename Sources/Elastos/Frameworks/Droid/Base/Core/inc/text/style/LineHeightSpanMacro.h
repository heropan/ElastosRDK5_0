#ifndef __LineHeightSpanMACRO_H__
#define __LineHeightSpanMACRO_H__

#define ILineHeightSpan_METHODS_DECL()                                  \
    CARAPI ChooseHeight(                                                \
        /* [in] */ ICharSequence* text,                                \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 end,                                           \
        /* [in] */ Int32 spanstartv,                                    \
        /* [in] */ Int32 v,                                             \
        /* [in] */ IPaintFontMetricsInt* fm);                          \


#define ILineHeightSpan_METHODS_IMPL(className, superClass, overRideClass)    \
ECode className::ChooseHeight(                                                \
    /* [in] */ ICharSequence* text,                                          \
    /* [in] */ Int32 start,                                                   \
    /* [in] */ Int32 end,                                                     \
    /* [in] */ Int32 spanstartv,                                              \
    /* [in] */ Int32 v,                                                       \
    /* [in] */ IPaintFontMetricsInt* fm)                                     \
{                                                                             \
    superClass::ChooseHeight(text, start, end, spanstartv, v, fm);            \
    return NOERROR;                                                           \
}                                                                             \

#endif  //__LineHeightSpanMACRO_H__