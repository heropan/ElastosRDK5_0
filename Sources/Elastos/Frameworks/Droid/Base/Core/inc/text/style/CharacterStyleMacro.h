#ifndef __CHARACTERSTYLEMACRO_H__
#define __CHARACTERSTYLEMACRO_H__

#define ICHARACTERSTYLE_METHODS_DECL()                                  \
    CARAPI GetUnderlying(                                               \
        /* [out] */ ICharacterStyle** result);                       \
                                                                        \
    CARAPI UpdateDrawState(                                             \
        /* [in] */ ITextPaint* tp);                                   \


#define ICHARACTERSTYLE_METHODS_IMPL(className, superClass, overRideClass)          \
ECode className::GetUnderlying(                                                     \
    /* [out] */ ICharacterStyle** ret)                                           \
{                                                                                   \
    VALIDATE_NOT_NULL(ret);                                                       \
    AutoPtr<ICharacterStyle> cs = superClass::GetUnderlying();                    \
    *ret = cs;                                                                   \
    INTERFACE_ADDREF(*ret);                                                        \
    return NOERROR;                                                                 \
}                                                                                   \
                                                                                    \
ECode className::UpdateDrawState(                                                   \
    /* [in] */ ITextPaint* tp)                                                    \
{                                                                                   \
    return superClass::UpdateDrawState(tp);                                               \
}                                                                                   \


#define ICharacterStyleHelper_METHODS_DECL()                                  \
    CARAPI Wrap(                                                              \
        /* [in] */ ICharacterStyle* cs,                                     \
        /* [out] */ ICharacterStyle** result);                             \


#define ICharacterStyleHelper_METHODS_IMPL(className, superClass, overRideClass)   \
ECode className::Wrap(                                                             \
    /* [in] */ ICharacterStyle* cs,                                              \
    /* [out] */ ICharacterStyle** ret)                                          \
{                                                                                  \
    VALIDATE_NOT_NULL(ret);                                                       \
    AutoPtr<ICharacterStyle> cs1 = superClass::Wrap(cs);                    \
    *ret = cs1;                                                                   \
    INTERFACE_ADDREF(*ret);                                                      \
    return NOERROR;                                                                \
}                                                                                  \


#endif  //__CHARACTERSTYLEMACRO_H__