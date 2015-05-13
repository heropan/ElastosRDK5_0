
#ifndef  __KEYFRAMEMACRO_H__
#define  __KEYFRAMEMACRO_H__

#define IKEYFRAME_METHOD_DECL()                                                \
    CARAPI HasValue(                                                            \
        /* [out] */ Boolean* hasValue);                                         \
                                                                                \
    CARAPI GetValue(                                                            \
        /* [out] */ IInterface** value);                                        \
                                                                                \
    CARAPI GetFraction(                                                         \
        /* [out] */ Float* fraction);                                           \
                                                                                \
    CARAPI SetFraction(                                                         \
        /* [in] */ Float fraction);                                             \
                                                                                \
    CARAPI GetInterpolator(                                                     \
        /* [out] */ ITimeInterpolator** interpolator);                          \
                                                                                \
    CARAPI SetInterpolator(                                                     \
        /* [in] */ ITimeInterpolator* interpolator);                            \
                                                                                \
    CARAPI GetType(                                                             \
        /* [out] */ ClassID* type);


#define IKEYFRAME_METHOD_IMPL(className, superClass)                           \
ECode className::HasValue(                                                      \
    /* [out] */ Boolean* hasValue)                                              \
{                                                                               \
    VALIDATE_NOT_NULL(hasValue);                                                \
    *hasValue = superClass::HasValue();                                         \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetValue(                                                      \
    /* [out] */ IInterface** value)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(value);                                                   \
    AutoPtr<IInterface> tmp = GetValue();                                      \
    *value = tmp;                                                               \
    INTERFACE_ADDREF(*value);                                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetFraction(                                                   \
    /* [out] */ Float* fraction)                                                \
{                                                                               \
    VALIDATE_NOT_NULL(fraction);                                                \
    *fraction = superClass::GetFraction();                                      \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetFraction(                                                   \
    /* [in] */ Float fraction)                                                  \
{                                                                               \
    return superClass::SetFraction(fraction);                                  \
}                                                                               \
                                                                               \
ECode className::GetInterpolator(                                               \
    /* [out] */ ITimeInterpolator** interpolator)                               \
{                                                                               \
    VALIDATE_NOT_NULL(interpolator);                                            \
    AutoPtr<ITimeInterpolator> tmp = superClass::GetInterpolator();             \
    *interpolator = tmp;                                                        \
    INTERFACE_ADDREF(*interpolator);                                            \
    return NOERROR;                                                            \
}                                                                               \
                                                                               \
ECode className::SetInterpolator(                                               \
    /* [in] */ ITimeInterpolator* interpolator)                                 \
{                                                                               \
    return superClass::SetInterpolator(interpolator);                          \
}                                                                               \
                                                                                \
ECode className::GetType(                                                       \
    /* [out] */ ClassID* type)                                              \
{                                                                               \
    VALIDATE_NOT_NULL(type);                                                    \
    *type = superClass::GetType();                                              \
    return NOERROR;                                                            \
}


#endif //__KEYFRAMEMACRO_H__
